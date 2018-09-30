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
    typedef real_t (*CustomFunction)(Variant start, Variant end, real_t duration, bool loop, bool pingpong, bool reverse);

    /**
     * Builtins should cover all cases.
     */
    enum Function {
	FUNCTION_NONE = 0,
	FUNCTION_LINEAR,
	FUNCTION_SQUARE,
	FUNCTION_CUBIC,
	FUNCTION_BEZIER,
	FUNCTION_SINE,
	FUNCTION_BOOLEAN,
	FUNCTION_EASE_BACK,
	FUNCTION_EASE_BOUNCE,
	FUNCTION_EASE_CIRC,
	FUNCTION_EASE_CUBIC,
	FUNCTION_EASE_ELASTIC,
	FUNCTION_EASE_EXPO,
	FUNCTION_EASE_QUAD,
	FUNCTION_EASE_QUART,
	FUNCTION_EASE_QUINT,
	FUNCTION_EASE_SINE,
	MAX_FUNCTION
    };

    /**
     * Register members here.
     */
    static void _register_methods();
    
    /**
     * Return the factor for tau
     * @param tau Interpolation where 0-1 is a complete cycle, although tau can be both < 0 and > 1.
     * @param loop The interpolation will repeat itself.
     * @param pingpong Include one reverse repitition within 1 tau.
     * @param reverse Reverse the whole thing.
     */
    real_t interpolation_factor(real_t tau, bool loop, bool pingpong, bool reverse);
    
    /**
     * Can be called as is without the Interpolation instance.
     * @param function What type of interpolation to use.
     * @param factor Interpolation factor.
     * @param start The start value.
     * @param end The end value.
     */
    real_t interpolate_scalar(int function, real_t factor, real_t start, real_t end);

    /**
     * Interpolate.
     * @param function What type of interpolation to use.
     * @param factor Interpolation factor.
     * @param start The start value, this value also determines the return type.
     * @param end The end value.
     */
    Variant interpolate(int function, real_t factor, Variant start, Variant end);
   
    Interpolation();
    ~Interpolation();

    /**
     * Similar to what _process() is for a node, but this is not a node.
     */
    void process(real_t delta);

    /**
     * Will be true if the interpolation is finished.
     * If the interpolation is looping this will always false.
     */
    bool is_finished(void);

    /**
     * Return the current value for the interpolation based on timestep and duration.
     */
    Variant get_value(void);

    
private:
    int _function;
    Variant _start;
    Variant _end;
    real_t _duration;
    bool _loop;
    bool _pingpong;
    bool _reverse;
    real_t _timestep;
    CustomFunction _custom;
};

    // VARIANT_ENUM_CAST(Interpolation::Function)
}

#endif
