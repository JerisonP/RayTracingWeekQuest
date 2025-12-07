#ifndef VEC3_H
#define VEC3_H

#include <cmath>     // For std::sqrt in length()
#include <iostream>  // For std::ostream in operator<<
#include <stdexcept> // For std::out_of_range in bounds-checked access and std::domain_error in div checks
#include <limits>    // For std::numeric_limits in near-zero checks

/*
 * A 3D vector class optimized for graphics/math computations.
 * Fixed-size array chosen for efficiency and cache friendliness.
 * Tip: For type safety, use composition or derivation for distinct types like points/colors to catch semantic errors at compile time.
 */

class vec3 {
	private:
	    double e[3];

	public:
	    /* Constructors */
	    vec3() : e{0, 0, 0} {}  // Default (zero-init for safe default behavior in ops)
	    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}  // Parameterized (explicit component control)

	    /* Component accessors */
	    // Inlined for low-overhead access in performance-critical code
	    double x() const { return e[0]; }
	    double y() const { return e[1]; }
	    double z() const { return e[2]; }

	    /* Operators */
	    vec3 operator-() const {
		return vec3(-e[0], -e[1], -e[2]);
	    }

	    // Subscript ops with runtime bounds checking via exceptions
	    // Trade-off: Adds slight overhead but prevents undefined behavior; consider unchecked versions or at() for flexibility
	    double operator[](int i) const {
		if (i < 0 || i > 2) {
		    throw std::out_of_range("vec3 index out of bounds (must be 0-2)");
		}
		return e[i];
	    }
	    double& operator[](int i) {
		if (i < 0 || i > 2) {
		    throw std::out_of_range("vec3 index out of bounds (must be 0-2)");
		}
		return e[i];
	    }

	    // Compound assignments enable op chaining
	    vec3& operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	    }
	    vec3& operator*=(double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	    }
	    vec3& operator/=(double t) {
		if (std::fabs(t) <= std::numeric_limits<double>::denorm_min()) {
		    throw std::domain_error("div-by-zero or divisor too small");
		}
		return *this *= 1/t;
	    }

	    /* Magnitude functions */
	    // length_squared preferred for comparisons to skip expensive sqrt
	    double length() const {
		return std::sqrt(length_squared());
	    }
	    double length_squared() const {
		return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
	    }
};

using point3 = vec3;

/* Non-member utilities */
// Placed outside class for ADL(Argument-Dependent-Lookup) and symmetric scalar ops

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return u + (-v);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.x(), t * v.y(), t * v.z());
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    if (std::fabs(t) <= std::numeric_limits<double>::denorm_min()) {
        throw std::domain_error("div-by-zero or divisor too small");
    }
    return v * (1/t);
}

inline double dot(const vec3& u, const vec3& v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.y() * v.z() - u.z() * v.y(),
                u.z() * v.x() - u.x() * v.z(),
                u.x() * v.y() - u.y() * v.x());
}

// Normalization; handles zero-length case to avoid NaN/div-by-zero
inline vec3 unit_vector(const vec3& v) {
    double len = v.length();
    if (std::fabs(len) <= std::numeric_limits<double>::denorm_min()) {
        throw std::domain_error("div-by-zero or length too small");
    }
    return v / len;
}

#endif
