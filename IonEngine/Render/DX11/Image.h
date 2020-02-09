#pragma once
#include "Render/Descriptors.h"

namespace IonEngine {
	class Image {
	public:
		const ImageDescriptor descriptor;

		Image(ImageDescriptor desc);
		Image(const Image&) = delete;
		void operator=(const Image&) = delete;

		//void setAPIResource(VkImage);
		//VkImage getAPIResource();

	private:

	};
}
