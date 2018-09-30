#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <Godot.hpp>
#include <Resource.hpp>

namespace godot {
    
class Interpolation : public godot::GodotScript<Resource> {
    GODOT_CLASS(Interpolation)
public:

    /**
     * Signature for custom interpolation functions.
     */
    typedef real_t (*CustomFunction)(Variant start, Variant difference, real_t timestep, real_t duration, bool loop, bool pingpong, bool reverse);

    /**
     * An operation the function should preform.
     */
    enum Operation {
	OPERATION_NONE = 0,
	OPERATION_IN,
	OPERATION_OUT,
	OPERATION_INOUT,
	MAX_OPERATION
    };
    
    /**
     * Builtins should cover all cases.
     */
    enum Function {
	FUNCTION_NONE = 0,
	FUNCTION_BACK,
	FUNCTION_BEZIER,
	FUNCTION_BOOLEAN,
	FUNCTION_BOUNCE,
	FUNCTION_CIRC,
	FUNCTION_CUBIC,
	FUNCTION_ELASTIC,
	FUNCTION_EXPO,
	FUNCTION_LINEAR,
	FUNCTION_QUAD,
	FUNCTION_QUART,
	FUNCTION_QUINT,
	FUNCTION_SINE,
	FUNCTION_SQUARE,
	FUNCTION_CUSTOM,
	MAX_FUNCTION
    };
    
    /**
     * Register members here.
     */
    static void _register_methods();

    /**
     * Get the operation name.
     */
    const char * operation_name(int operation);
    
    /**
     * Get the function name.
     */
    const char * function_name(int function);
    
    /**
     * Get the factor for tau.
     * @param function What type of interpolation to use.
     * @param operation The operation to preform.
     * @param tau The current position in the cycle, any value is acceptable including negative and > 1.
     * @param loop True to modulo tau with 1.
     * @param reverse Reverse the whole thing.
     * @returns The factor the function will use, will always be between 0 to 1.
     */
    real_t interpolation_factor(int function, int operation, real_t tau, bool loop, bool reverse);
    
    /**
     * Mimics the signature of Robert Penner's functions.
     * @param function What type of interpolation to use.
     * @param operation The operation to preform.
     * @param timestep The current time.
     * @param start The start value.
     * @param difference Change in value.
     * @param duration The duration.
     */
    real_t interpolate_scalar_penner(int function, int operation, real_t timestep, real_t start, real_t difference, real_t duration);
    
    /**
     * Interpolate.
     * @param function What type of interpolation to use.
     * @param operation The operation to preform.
     * @param tau The current position in the cycle, any value is acceptable including negative and > 1.
     * @param start The start value, will also determine the return type (except for integers which will always return a real).
     * @param end The end value.
     * @param loop True to modulo tau with 1.
     * @param reverse Reverse the whole thing.
     * @returns The interpolated value.
     *
     * The following variants will be interpolated:
     *
     * - integer or real with another integer or real: interpolation to a real.
     * - int/real (int/real): interpolates to a real.
     * - Vector2 with another Vector2 or Vector3: interpolation to a Vector2.
     * - Rect2 with another Rect2: interpolation to a Rect2.
     * - Color with another Color or Vector3: interpolation to a Color.
     * - Quaternion with another Quaternion, Basis or Transform: spherical interpolation to Quaternion.
     * - Basis with another Basis, Quaternion or Transform: spherical interpolation to Basis.
     * - Transform with another Transform, Quaternion, Basis or Vector3: 
     *   - In the case of interpolating with Quaternion or Basis: spherical interpolation to Transform (rotation only).
     *   - In the case of interpolating with Vector3: interpolation to Transform (translation only).
     *   - Another Transform: spherical interpolation (rotation) and interpolation (translation) to Transform.
     * - Transform2D with another Transform2D, Vector2, real or int: 
     *   - In the case of interpolating with real or int: spherical interpolation to Transform2D (rotation only).
     *   - In the case of interpolating with Vector2: interpolation to Transform2D (translation only).
     *   - Another Transform: spherical interpolation (rotation) and interpolation (translation) to Transform2D.
     *
     * TODO: add more types.
     */
    Variant interpolate(int function, int operation, real_t tau, Variant start, Variant end, bool loop, bool reverse);
   
    Interpolation();
    ~Interpolation();

    /**
     * Return the current value for the interpolation based on timestep and duration.
     */
    Variant get_value(void);

    
private:
    int _function;
    int _operation;
    Variant _start;
    Variant _end;
    real_t _duration;
    bool _loop;
    bool _reverse;
    real_t _timestep;
    CustomFunction _custom;
};

    // VARIANT_ENUM_CAST(Interpolation::Function)
}

#endif
