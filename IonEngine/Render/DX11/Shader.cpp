#include "Shader.h"

#include <d3dcompiler.h>
using namespace IonEngine;



Shader::Shader() {
	vertexShader = nullptr;
	pixelShader = nullptr;
	layout = nullptr;
	matrixBuffer = nullptr;
}

Shader::~Shader() {

}

bool Shader::initialize(ID3D11Device* device, HWND hwnd) {
	bool success = initializeShader(device, hwnd, L"res/shaders/Color.vs.hlsl", L"res/shaders/Color.ps.hlsl");
	if (!success) {
		return false;
	}

	return true;
}

void Shader::shutdown() {
	shutdownShader();
}

bool Shader::render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix) {
	bool success = setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!success) {
		return false;
	}

	renderShader(deviceContext, indexCount);

	return true;
}

bool Shader::initializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename) {
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	u32 numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// Compile the vertex shader code
	result = D3DCompileFromFile(vsFilename, nullptr, nullptr, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result)) {
		// If the shader failed to compile it should have writen something to the error message
		if (errorMessage != nullptr) {
			outputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		} else { // If there was nothing in the error message then it simply could not find the shader file itself
			MessageBoxW(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	// Compile the pixel shader code
	result = D3DCompileFromFile(psFilename, nullptr, nullptr, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result)) {
		// If the shader failed to compile it should have writen something to the error message
		if (errorMessage != nullptr) {
			outputShaderErrorMessage(errorMessage, hwnd, psFilename);
		} else { // If there was nothing in the error message then it simply could not find the shader file itself
			MessageBoxW(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	// Create the vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &vertexShader);
	if (FAILED(result)) {
		return false;
	}
	// Create the pixel shader from the buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &pixelShader);
	if (FAILED(result)) {
		return false;
	}

	// Create the vertex input layout description
	// this setup needs to match the VertexType structure in the ModelClass and in the shader
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	// Create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &layout);
	if (FAILED(result)) {
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// Setup the description of the dynamic matrix constant buffer that is the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &matrixBuffer);
	if (FAILED(result)) {
		return false;
	}

	return true;
}

void Shader::shutdownShader() {
	if (matrixBuffer != nullptr) {
		matrixBuffer->Release();
		matrixBuffer = nullptr;
	}

	if (layout != nullptr) {
		layout->Release();
		layout = nullptr;
	}

	if (pixelShader != nullptr) {
		pixelShader->Release();
		pixelShader = nullptr;
	}

	if (vertexShader != nullptr) {
		vertexShader->Release();
		vertexShader = nullptr;
	}
}

void Shader::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename) {
	char* compileErrors;
	u64 bufferSize, i;
	std::ofstream fout;

	// Get a pointer to the error message text buffer
	compileErrors = (char*) errorMessage->GetBufferPointer();
	// Get the length of the message
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error messageto
	fout.open("shader-error.txt");
	// Write out the error message
	for (i = 0; i < bufferSize; i++) {
		fout << compileErrors[i];
	}
	// Close the file
	fout.close();

	// Release the error message
	errorMessage->Release();
	errorMessage = nullptr;

	// Pop a message to notify the user to check the text file for compile errors
	MessageBoxW(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFilename, MB_OK);
}

bool Shader::setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix) {
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBuffer* dataPtr;
	u32 bufferNumber;

	// Transpose the matrices
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}
	// Get a pointer to the data in the constant buffer
	dataPtr = (MatrixBuffer*) mappedResource.pData;
	// Copy the matrices into the constant buffer
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	// Unlock the constant buffer
	deviceContext->Unmap(matrixBuffer, 0);

	//Set the position of the constant buffer in the vertex shader
	bufferNumber = 0;
	// Finaly set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

	return true;
}

void Shader::renderShader(ID3D11DeviceContext* deviceContext, int indexCount) {
	// Set the vertex input layout
	deviceContext->IASetInputLayout(layout);

	// Set the vertex and pixel shaders that will be used to render this triangle
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	// Render the test triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
