#include "XMath.h"

#include <string>



/* ==== CONSTANTS ==== */
// Vec2i
const Vec2i Vec2i::zero = {0, 0};
const Vec2i Vec2i::one = {1, 1};
const Vec2i Vec2i::right = {1, 0};
const Vec2i Vec2i::left = {-1, 0};
const Vec2i Vec2i::up = {0, 1};
const Vec2i Vec2i::down = {0, -1};
const Vec2i Vec2i::max = {INT_MAX, INT_MAX};
const Vec2i Vec2i::min = {INT_MIN, INT_MIN};
// Vec3i
const Vec3i Vec3i::zero = {0, 0, 0};
const Vec3i Vec3i::one = {1, 1, 1};
const Vec3i Vec3i::right = {1, 0, 0};
const Vec3i Vec3i::left = {-1, 0, 0};
const Vec3i Vec3i::up = {0, 1, 0};
const Vec3i Vec3i::down = {0, -1, 0};
const Vec3i Vec3i::forward = {0, 0, 1};
const Vec3i Vec3i::backward = {0, 0, -1};
const Vec3i Vec3i::max = {INT_MAX, INT_MAX, INT_MAX};
const Vec3i Vec3i::min = {INT_MIN, INT_MIN, INT_MIN};
const Vec3i Vec3i::north = {0, 0, 1};
const Vec3i Vec3i::east = {1, 0, 0};
const Vec3i Vec3i::south = {0, 0, -1};
const Vec3i Vec3i::west = {-1, 0, 0};
// Vec4i
const Vec4i Vec4i::zero = {0, 0, 0, 0};
const Vec4i Vec4i::one = {1, 1, 1, 1};
const Vec4i Vec4i::one3 = {1, 1, 1, 0};
const Vec4i Vec4i::right = {1, 0, 0, 0};
const Vec4i Vec4i::left = {-1, 0, 0, 0};
const Vec4i Vec4i::up = {0, 1, 0, 0};
const Vec4i Vec4i::down = {0, -1, 0, 0};
const Vec4i Vec4i::forward = {0, 0, 1, 0};
const Vec4i Vec4i::backward = {0, 0, -1, 0};
const Vec4i Vec4i::max = {INT_MAX, INT_MAX, INT_MAX, INT_MAX};
const Vec4i Vec4i::min = {INT_MIN, INT_MIN, INT_MIN, INT_MIN};
// Vec2f
const Vec2f Vec2f::zero = {0, 0};
const Vec2f Vec2f::one = {1, 1};
const Vec2f Vec2f::right = {1, 0};
const Vec2f Vec2f::left = {-1, 0};
const Vec2f Vec2f::up = {0, 1};
const Vec2f Vec2f::down = {0, -1};
const Vec2f Vec2f::positiveInfinity = {IonEngine::Math::positiveInfinity, IonEngine::Math::positiveInfinity};
const Vec2f Vec2f::negativeInfinity = {IonEngine::Math::negativeInfinity, IonEngine::Math::negativeInfinity};
// Vec3f
const Vec3f Vec3f::zero = {0, 0, 0};
const Vec3f Vec3f::one = {1, 1, 1};
const Vec3f Vec3f::right = {1, 0, 0};
const Vec3f Vec3f::left = {-1, 0, 0};
const Vec3f Vec3f::up = {0, 1, 0};
const Vec3f Vec3f::down = {0, -1, 0};
const Vec3f Vec3f::forward = {0, 0, 1};
const Vec3f Vec3f::backward = {0, 0, -1};
const Vec3f Vec3f::positiveInfinity = {IonEngine::Math::positiveInfinity, IonEngine::Math::positiveInfinity, IonEngine::Math::positiveInfinity};
const Vec3f Vec3f::negativeInfinity = {IonEngine::Math::negativeInfinity, IonEngine::Math::negativeInfinity, IonEngine::Math::negativeInfinity};
// Vec4f
const Vec4f Vec4f::zero = {0, 0, 0, 0};
const Vec4f Vec4f::zeroPoint = {0, 0, 0, 1};
const Vec4f Vec4f::one = {1, 1, 1, 1};
const Vec4f Vec4f::oneDir = {1, 1, 1, 0};
const Vec4f Vec4f::right = {1, 0, 0, 0};
const Vec4f Vec4f::left = {-1, 0, 0, 0};
const Vec4f Vec4f::up = {0, 1, 0, 0};
const Vec4f Vec4f::down = {0, -1, 0, 0};
const Vec4f Vec4f::forward = {0, 0, 1, 0};
const Vec4f Vec4f::backward = {0, 0, -1, 0};
const Vec4f Vec4f::positiveInfinity = {IonEngine::Math::positiveInfinity, IonEngine::Math::positiveInfinity, IonEngine::Math::positiveInfinity, IonEngine::Math::positiveInfinity};
const Vec4f Vec4f::negativeInfinity = {IonEngine::Math::negativeInfinity, IonEngine::Math::negativeInfinity, IonEngine::Math::negativeInfinity, IonEngine::Math::negativeInfinity};
const Vec4f Vec4f::positiveInfinityPoint = {IonEngine::Math::positiveInfinity, IonEngine::Math::positiveInfinity, IonEngine::Math::positiveInfinity, 1};
const Vec4f Vec4f::negativeInfinityPoint = {IonEngine::Math::negativeInfinity, IonEngine::Math::negativeInfinity, IonEngine::Math::negativeInfinity, 1};
const Vec4f Vec4f::positiveInfinityDir = {IonEngine::Math::positiveInfinity, IonEngine::Math::positiveInfinity, IonEngine::Math::positiveInfinity, 0};
const Vec4f Vec4f::negativeInfinityDir = {IonEngine::Math::negativeInfinity, IonEngine::Math::negativeInfinity, IonEngine::Math::negativeInfinity, 0};
// Rotor3f
const Rotor3f Rotor3f::identity = {1, {0, 0, 0}};
// Matrix4x4f
const Matrix4x4f Matrix4x4f::identity = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};
// Quaternion
const Quaternion Quaternion::identity = {0.0f, 0.0f, 0.0f, 1.0f};
// Color
const Color Color::clear		= Color(0, 0, 0, 0);
const Color Color::white		= Color(1, 1, 1, 1);
const Color Color::black		= Color(0, 0, 0, 1);
const Color Color::lightGray	= Color(.666f, .666f, .666f, 1);
const Color Color::darkGray		= Color(.333f, .333f, .333f, 1);
const Color Color::blue			= Color(0, 0, 1, 1);
const Color Color::green		= Color(0, 1, 0, 1);
const Color Color::cyan			= Color(0, 1, 1, 1);
const Color Color::red			= Color(1, 0, 0, 1);
const Color Color::magenta		= Color(1, 0, 1, 1);
const Color Color::yellow		= Color(1, 1, 0, 1);
const Color Color::orange		= Color(1, .5f, 0, 1);
const Color Color::lime			= Color(.5f, 1, 0, 1);
const Color Color::turquoise	= Color(0, 1, .5f, 1);
const Color Color::sky			= Color(0, .5f, 1, 1);
const Color Color::purple		= Color(.5f, 0, 1, 1);
const Color Color::pink			= Color(1, 0, .5f, 1);


/* ==== METHODS ==== */
/* ---- Vectors ---- */
// Vec2i
const char* Vec2i::toCString() {
	return toString().c_str();
}
std::string Vec2i::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

// Vec3i
const char* Vec3i::toCString() {
	return toString().c_str();
}
std::string Vec3i::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
}

// Vec4i
const char* Vec4i::toCString() {
	return toString().c_str();
}
std::string Vec4i::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
}

// Vec2f
const char* Vec2f::toCString() {
	return toString().c_str();
}
std::string Vec2f::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

// Vec3f
const char* Vec3f::toCString() {
	return toString().c_str();
}
std::string Vec3f::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
}

// Vec4f
const char* Vec4f::toCString() {
	return toString().c_str();
}
std::string Vec4f::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
}

// Bivec3f
const char* Bivec3f::toCString() {
	return toString().c_str();
}
std::string Bivec3f::toString() {
	return "(xy = " + std::to_string(xy) + ", xz = " + std::to_string(xz) + ", yz = " + std::to_string(yz) + ")";
}

// Rotor3f
const char* Rotor3f::toCString() {
	return toString().c_str();
}
std::string Rotor3f::toString() {
	return "Rotor3f(a = " + std::to_string(a) + ", bv = " + std::string(bv.toString()) + ")";
}

// Quaternion
const char* Quaternion::toCString() {
	return toString().c_str();
}
std::string Quaternion::toString() {
	return "Quaternion(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
}


/* ---- Matrices ----*/
// Matrix4x4f
void Matrix4x4f::luDecomposition(Matrix4x4f &lu) {

	float sum = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = i; j < 4; j++) {
			sum = 0;
			for (int k = 0; k < i; k++) {
				sum += lu.at(i, k) * lu.at(k, j);
			}
			lu.at(i, j) = at(i, j) - sum;
		}
		for (int j = i + 1; j < 4; j++) {
			sum = 0;
			for (int k = 0; k < i; k++) {
				sum += lu.at(i, k) * lu.at(k, i);
			}
			lu.at(j, i) = (1.0f / lu.at(i, i)) * (at(j, i) - sum);
		}
	}

}

Vec4f Matrix4x4f::solve(Matrix4x4f &lu, Vec4f toSolve) {

	float sum = 0;
	// find solution of Ly = b
	Vec4f y = {0, 0, 0, 0};
	for (int i = 0; i < 4; i++) {
		sum = 0;
		for (int k = 0; k < i; k++) {
			sum += lu.at(i, k) * y[k];
		}
		y[i] = toSolve[i] - sum;
	}
	// find solution of Ux = y
	Vec4f x = {0, 0, 0, 0};
	for (int i = 3; i >= 0; i--) {
		sum = 0;
		for (int k = i + 1; k < 4; k++) {
			sum += lu.at(i, k) * x[k];
		}
		x[i] = (1.0f / lu.at(i, i)) * (y[i] - sum);
	}

	return x;
}

const char* Matrix4x4f::toCString() {
	return (
		std::to_string(m00) + ", " + std::to_string(m01) + ", " + std::to_string(m02) + ", " + std::to_string(m03) + ",\n" +
		std::to_string(m10) + ", " + std::to_string(m11) + ", " + std::to_string(m12) + ", " + std::to_string(m13) + ",\n" +
		std::to_string(m20) + ", " + std::to_string(m21) + ", " + std::to_string(m22) + ", " + std::to_string(m23) + ",\n" +
		std::to_string(m30) + ", " + std::to_string(m31) + ", " + std::to_string(m32) + ", " + std::to_string(m33) + "\n"
	).c_str();
}
std::string Matrix4x4f::toString() {
	return (
		std::to_string(m00) + ", " + std::to_string(m01) + ", " + std::to_string(m02) + ", " + std::to_string(m03) + ",\n" +
		std::to_string(m10) + ", " + std::to_string(m11) + ", " + std::to_string(m12) + ", " + std::to_string(m13) + ",\n" +
		std::to_string(m20) + ", " + std::to_string(m21) + ", " + std::to_string(m22) + ", " + std::to_string(m23) + ",\n" +
		std::to_string(m30) + ", " + std::to_string(m31) + ", " + std::to_string(m32) + ", " + std::to_string(m33) + "\n"
	);
}

/* ---- Others ---- */
// Color
const char* Color::toCString() {
	return toString().c_str();
}
std::string Color::toString() {
	return "(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a) + ")";
}

// Boxi
const char* Boxi::toCString() {
	return toString().c_str();
}
std::string Boxi::toString() {
	return "Boxi(min" + std::string(min.toString()) + ", max" + std::string(max.toString()) + ")";
}

bool Boxi::intersect(const Boxi& o) const {
	return !(o.min.x > max.x || o.max.x < min.x) && !(o.min.y > max.y || o.max.y < min.y) && !(o.min.z > max.z || o.max.z < min.z);
}
bool Boxi::intersect(const Boxf& o) const {
	return !(o.min.x > max.x || o.max.x < min.x) && !(o.min.y > max.y || o.max.y < min.y) && !(o.min.z > max.z || o.max.z < min.z);
}
bool Boxi::intersect(const Ray3f& ray) const {
	return Boxf(toVec3f(min), toVec3f(max)).intersect(ray);
}
bool Boxi::intersect(const Line3f& line) const {
	return Boxf(toVec3f(min), toVec3f(max)).intersect(line);
}
bool Boxi::intersect(const Segment3f& segment) const {
	return Boxf(toVec3f(min), toVec3f(max)).intersect(segment);
}
bool Boxi::intersect(const Plane& plane) const {
	return Boxf(toVec3f(min), toVec3f(max)).intersect(plane);
}

bool Boxi::intersect(const Boxi& o, Boxi& i) const {
	i.min.x = maxi(min.x, o.min.x);
	i.min.y = maxi(min.y, o.min.y);
	i.min.z = maxi(min.z, o.min.z);

	i.max.x = mini(max.x, o.max.x);
	i.max.y = mini(max.y, o.max.y);
	i.max.z = mini(max.z, o.max.z);

	return i.min.x < i.max.x && i.min.y < i.max.y && i.min.z < i.max.z;
}
bool Boxi::intersect(const Boxf& o, Boxf& i) const {
	i.min.x = maxf(min.x, o.min.x);
	i.min.y = maxf(min.y, o.min.y);
	i.min.z = maxf(min.z, o.min.z);

	i.max.x = minf(max.x, o.max.x);
	i.max.y = minf(max.y, o.max.y);
	i.max.z = minf(max.z, o.max.z);

	return i.min.x < i.max.x && i.min.y < i.max.y && i.min.z < i.max.z;
}
bool Boxi::intersect(const Ray3f& ray, Vec3f& i) const {
	return Boxf(toVec3f(min), toVec3f(max)).intersect(ray, i);
}
bool Boxi::intersect(const Line3f& line, Vec3f& i) const {
	return Boxf(toVec3f(min), toVec3f(max)).intersect(line, i);
}
bool Boxi::intersect(const Segment3f& segment, Vec3f& i) const {
	return Boxf(toVec3f(min), toVec3f(max)).intersect(segment, i);
}
bool Boxi::intersect(const Plane& plane, float& side) const {
	return Boxf(toVec3f(min), toVec3f(max)).intersect(plane, side);
}

// Boxf
const char* Boxf::toCString() {
	return toString().c_str();
}
std::string Boxf::toString() {
	return "Boxf(min" + std::string(min.toString()) + ", max" + std::string(max.toString()) + ")";
}

bool Boxf::intersect(const Boxi& o) const {
	return !(o.min.x > max.x || o.max.x < min.x) && !(o.min.y > max.y || o.max.y < min.y) && !(o.min.z > max.z || o.max.z < min.z);
}
bool Boxf::intersect(const Boxf& o) const {
	// TODO find faster test
	return !(o.min.x > max.x || o.max.x < min.x) && !(o.min.y > max.y || o.max.y < min.y) && !(o.min.z > max.z || o.max.z < min.z);
	//return oMin.x < max.x && oMax.x > min.x && oMin.y < max.y && oMax.y > min.y && oMin.z < max.z && oMax.z > min.z;
}
bool Boxf::intersect(const Ray3f& ray) const {
	float dist = 0.0f;
	return intersect(ray.origin, ray.direction, dist) && dist > 0.0f;
}
bool Boxf::intersect(const Line3f& line) const {
	float dist = 0.0f;
	Vec3f lineDir = line.p2 - line.p1;
	lineDir.normalize();
	return intersect(line.p1, lineDir, dist);
}
bool Boxf::intersect(const Segment3f& segment) const {
	float dist = 0.0f;
	Vec3f segmentVec = segment.p2 - segment.p1;
	return intersect(segment.p1, segmentVec.normalized(), dist) && dist > 0.0f && dist < segmentVec.magnitude();
}
bool Boxf::intersect(const Plane& plane) const {
	Vec3f extents = getExtents();
	Vec3f center = min + extents;

	float r = extents.x * fabsf(plane.normal.x) + extents.y * fabsf(plane.normal.y) + extents.z * fabsf(plane.normal.z);
	float s = plane.normal.dot(center) - plane.distance;

	return fabsf(s) <= r;
}

bool Boxf::intersect(const Boxi& o, Boxf& i) const {
	return o.intersect(*this, i);
}
bool Boxf::intersect(const Boxf& o, Boxf& i) const {
	i.min.x = maxf(min.x, o.min.x);
	i.min.y = maxf(min.y, o.min.y);
	i.min.z = maxf(min.z, o.min.z);

	i.max.x = minf(max.x, o.max.x);
	i.max.y = minf(max.y, o.max.y);
	i.max.z = minf(max.z, o.max.z);

	return i.min.x < i.max.x && i.min.y < i.max.y && i.min.z < i.max.z;
}
bool Boxf::intersect(const Ray3f& ray, Vec3f& i) const {
	float dist = 0.0f;
	if (intersect(ray.origin, ray.direction, dist) && dist > 0.0f) {
		i = ray.origin + ray.direction * dist;
		return true;
	}
	return false;
}
bool Boxf::intersect(const Line3f& line, Vec3f& i) const {
	float dist = 0.0f;
	Vec3f lineDir = line.p2 - line.p1;
	lineDir.normalize();
	if (intersect(line.p1, lineDir, dist)) {
		i = line.p1 + lineDir * dist;
		return true;
	}
	return false;
}
bool Boxf::intersect(const Segment3f& segment, Vec3f& i) const {
	float dist = 0.0f;
	Vec3f segmentVec = segment.p2 - segment.p1;
	if (intersect(segment.p1, segmentVec.normalized(), dist) && dist > 0.0f && dist < segmentVec.magnitude()) {
		i =	segment.p1 + segmentVec.normalized() * dist;
		return true;
	}
	return false;
}
bool Boxf::intersect(const Plane& plane, float& side) const {
	Vec3f extents = getExtents();
	Vec3f center = min + extents;

	float r = extents.x * fabsf(plane.normal.x) + extents.y * fabsf(plane.normal.y) + extents.z * fabsf(plane.normal.z);
	float s = plane.normal.dot(center) - plane.distance;

	side = (s < 0) ? -1.0f : 1.0f;
	return fabsf(s) <= r;
}

bool Boxf::intersect(const Vec3f& o, const Vec3f& d, float& t) const {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	Vec3f bounds[2] = {min, max};

	Vec3f invdir = 1.0f / d;
	Vec3i raySign = Vec3i(invdir.x < 0, invdir.y < 0, invdir.z < 0);

	tmin = (bounds[raySign[0]].x - o.x) * invdir.x;
	tmax = (bounds[1 - raySign[0]].x - o.x) * invdir.x;
	tymin = (bounds[raySign[1]].y - o.y) * invdir.y;
	tymax = (bounds[1 - raySign[1]].y - o.y) * invdir.y;

	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (bounds[raySign[2]].z - o.z) * invdir.z;
	tzmax = (bounds[1 - raySign[2]].z - o.z) * invdir.z;

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	t = tmin;
	return true;
}

// Recti
const char* Recti::toCString() {
	return toString().c_str();
}
std::string Recti::toString() {
	return "Recti(min" + std::string(min.toString()) + ", max" + std::string(max.toString()) + ")";
}

// Rectf
const char* Rectf::toCString() {
	return toString().c_str();
}
std::string Rectf::toString() {
	return "Rectf(min" + std::string(min.toString()) + ", max" + std::string(max.toString()) + ")";
}

// Ray2f
const char* Ray2f::toCString() {
	return toString().c_str();
}
std::string Ray2f::toString() {
	return "Ray2f(origin" + std::string(origin.toString()) + ", direction" + std::string(direction.toString()) + ")";
}

// Ray3f
const char* Ray3f::toCString() {
	return toString().c_str();
}
std::string Ray3f::toString() {
	return "Ray3f(origin" + std::string(origin.toString()) + ", direction" + std::string(direction.toString()) + ")";
}

// Line2f
const char* Line2f::toCString() {
	return toString().c_str();
}
std::string Line2f::toString() {
	return "Line2f(" + p1.toString() + ", " + p2.toString() + ")";
}

// Line3f
const char* Line3f::toCString() {
	return toString().c_str();
}
std::string Line3f::toString() {
	return "Line3f(" + p1.toString() + ", " + p2.toString() + ")";
}

// Segment2f
const char* Segment2f::toCString() {
	return toString().c_str();
}
std::string Segment2f::toString() {
	return "Segment2f(" + p1.toString() + ", " + p2.toString() + ")";
}

// Segment3f
const char* Segment3f::toCString() {
	return toString().c_str();
}
std::string Segment3f::toString() {
	return "Segment3f(" + p1.toString() + ", " + p2.toString() + ")";
}

// Plane
const char* Plane::toCString() {
	return toString().c_str();
}
std::string Plane::toString() {
	return "Plane(normal" + std::string(normal.toString()) + ", distance = " + std::to_string(distance) + ")";
}

Plane Plane::transform(const Matrix4x4f& transf) {
	Matrix4x4f inv = transf.inverseAffine();
	inv.transpose();
	return Plane(inv * normalDistance);
}

bool Plane::intersect(const Boxi& box) const {
	return box.intersect(*this);
}
bool Plane::intersect(const Boxf& box) const {
	return box.intersect(*this);
}
bool Plane::intersect(const Ray3f& ray) const {
	float t = 0.0f;
	return intersect(ray.origin, ray.direction, t) && t >= 0.0f;
}
bool Plane::intersect(const Line3f& line) const {
	float t;
	return intersect(line.p1, (line.p2 - line.p1).normalized(), t);
}
bool Plane::intersect(const Segment3f& segment) const {
	float t;
	Vec3f segmentDir = segment.p2 - segment.p1;
	float segLength = segmentDir.magnitude();
	segmentDir /= segLength;
	return intersect(segment.p1, segmentDir, t) && t >= 0.0f && t <= segLength;
}

bool Plane::intersect(const Boxi& box, float& side) const {
	return box.intersect(*this, side);
}
bool Plane::intersect(const Boxf& box, float& side) const {
	return box.intersect(*this, side);
}
bool Plane::intersect(const Ray3f& ray, Vec3f& i) const {
	float t = 0.0f;
	if (intersect(ray.origin, ray.direction, t) && t >= 0.0f) {
		i = ray.origin + ray.direction * t;
		return true;
	}
	return false;
}
bool Plane::intersect(const Line3f& line, Vec3f& i) const {
	float t = 0.0f;
	Vec3f lineDir = line.p2 - line.p1;
	lineDir.normalize();
	if (intersect(line.p1, lineDir, t)) {
		i = line.p1 + lineDir * t;
		return true;
	}
	return false;
}
bool Plane::intersect(const Segment3f& segment, Vec3f& i) const {
	float t = 0.0f;
	Vec3f segmentDir = segment.p2 - segment.p1;
	float segLength = segmentDir.magnitude();
	segmentDir /= segLength;
	if (intersect(segment.p1, segmentDir, t) && t >= 0.0f && t <= segLength) {
		i = segment.p1 + segmentDir * t;
		return true;
	}
	return false;
}

bool Plane::intersect(const Vec3f& origin, const Vec3f& direction, float& t) const {
	float denom = normal.dot(direction);
	if (denom > 1e-6) {
		Vec3f po = (normal * distance) - origin;
		t = po.dot(normal) / denom;
		return true;
	}

	return false;
}

Vec3f Plane::project(const Vec3f& v) const {
	return v - v.dot(normal) * normal;
}

Vec3f Plane::projectPoint(const Vec3f& p) const {
	return project(p) + normal * distance;
}

// OBB2D
const char* OBB2D::toCString() {
	return toString().c_str();
}
std::string OBB2D::toString() {
	return "OBB2D(center" + center.toString() + ", size" + size.toString() + ", rotation = " + std::to_string(rotation) + ")";
}

bool OBB2D::raycast(const Ray2f& ray, float& distance) const {
	distance = INFINITY;
	Vec2f delta = center - ray.origin;

	float tMin = 0.0f;
	float tMax = INFINITY;
	for (int a = 0; a < 2; a++) {
		float e = axis[a].dot(delta);
		float f = ray.direction.dot(axis[a]);

		if (f > 0.001f || f < -0.001f) {
			float t1 = (e - size[a] * 0.5f) / f;
			float t2 = (e + size[a] * 0.5f) / f;

			swapToMinMax(t1, t2);

			if (t2 < tMax) {
				tMax = t2;
			}
			if (t1 > tMin) {
				tMin = t1;
			}

			if (tMax < tMin) {
				return false;
			}
		} else {
			if (-e - size[a] * 0.5f > 0.0f || -e + size[a] * 0.5f < 0.0f) {
				return false;
			}
		}
	}

	distance = tMin;
	return true;
}

bool OBB2D::overlaps(const OBB2D& other) const {
	return overlaps1Way(other) && other.overlaps1Way(*this);
}

bool OBB2D::overlaps1Way(const OBB2D& other) const {
	float tMin = -INFINITY;
	float tMax = INFINITY;
	for (int a = 0; a < 2; a++) {
		for (int c = 0; c < 4; c++) {
			float t = other.corner[c].dot(axis[a]);

			if (t < tMin) {
				tMin = t;
			} else if (t > tMax) {
				tMax = t;
			}
		}

		if (tMin > 1 || tMax < 0) {
			return false;
		}
	}
	return true;
}

// OBB3D
const char* OBB3D::toCString() {
	return toString().c_str();
}
std::string OBB3D::toString() {
	return "OBB3D(center" + center.toString() + ", size" + size.toString() + ", rotation" + rotation.toString() + ")";
}

// Frustum3f
const char* Frustum3f::toCString() {
	return toString().c_str();
}
std::string Frustum3f::toString() {
	return "Frustum(near=" + std::to_string(near) + ", far=" + std::to_string(far) + ", horiFOV=" + std::to_string(horiFOV) + ", aspect=" + std::to_string(aspect) + ")";
}

void Frustum3f::updatePlanes(const Matrix4x4f& viewMatrix) {
	Matrix4x4f combined = projectionMatrix * viewMatrix;
	Matrix4x4f inv = viewMatrix.inverseAffine() * projectionMatrix.inversePerspective();
	Vec3f nbl = (inv * Vec4f(-near, -near, -near, near)).xyz; // nbl
	Vec3f ntl = (inv * Vec4f(-near, near, -near, near)).xyz; // ntl
	Vec3f ntr = (inv * Vec4f(near, near, -near, near)).xyz; // ntr
	Vec3f nbr = (inv * Vec4f(near, -near, -near, near)).xyz; // nbr
	Vec3f fbl = (inv * Vec4f(-far, -far, far, far)).xyz; // fbl
	Vec3f ftl = (inv * Vec4f(-far, far, far, far)).xyz; // ftl
	Vec3f ftr = (inv * Vec4f(far, far, far, far)).xyz; // ftr
	Vec3f fbr = (inv * Vec4f(far, -far, far, far)).xyz; // fbr
	leftPlane = Plane(nbl, ntl, ftl);
	rightPlane = Plane(nbr, fbr, ftr);
	bottomPlane = Plane(nbl, fbr, nbr);
	topPlane = Plane(ntl, ftr, ftl);
	nearPlane = Plane(nbl, nbr, ntl);
	farPlane = Plane(fbr, ftl, ftr);

	// TODO use method to extract from combined viewProjection matrix directly
	//leftPlane = Plane(Vec4f(combined.m30 + combined.m00, combined.m31 + combined.m01, combined.m32 + combined.m02, combined.m33 + combined.m03));
	//rightPlane = Plane(Vec4f(combined.m30 - combined.m00, combined.m31 - combined.m01, combined.m32 - combined.m02, combined.m33 - combined.m03));
	//bottomPlane = Plane(Vec4f(combined.m30 + combined.m10, combined.m31 + combined.m11, combined.m32 + combined.m12, combined.m33 + combined.m13));
	//topPlane = Plane(Vec4f(combined.m30 - combined.m10, combined.m31 - combined.m11, combined.m32 - combined.m12, combined.m33 - combined.m13));
	//nearPlane = Plane(Vec4f(combined.m30 + combined.m20, combined.m31 + combined.m21, combined.m32 + combined.m22, combined.m33 + combined.m23));
	//farPlane = Plane(Vec4f(combined.m30 - combined.m20, combined.m31 - combined.m21, combined.m32 - combined.m22, combined.m33 - combined.m23));
	//Matrix4x4f planeTransf = viewMatrix.transposed();
	//leftPlane.normalDistance = planeTransf * leftPlane.normalDistance;
	//rightPlane.normalDistance = planeTransf * rightPlane.normalDistance;
	//bottomPlane.normalDistance = planeTransf * bottomPlane.normalDistance;
	//topPlane.normalDistance = planeTransf * topPlane.normalDistance;
	//nearPlane.normalDistance = planeTransf * nearPlane.normalDistance;
	//farPlane.normalDistance = planeTransf * farPlane.normalDistance;

	leftPlane.normalize();
	rightPlane.normalize();
	bottomPlane.normalize();
	topPlane.normalize();
	nearPlane.normalize();
	farPlane.normalize();
}

bool Frustum3f::intersect(const Boxf& box) const {
	float side;
	for (int i = 0; i < 6; i++) {
		if (!planes[i].intersect(box, side) && side > 0.0f) { // Does not intersect and is in front
			return false;
		}
	}
	return true;
}
