#include "interpolation.h"

using namespace godot;

void Interpolation::_register_methods()
{
    register_method((char *)"interpolation_factor", &Interpolation::interpolation_factor);
    register_method((char *)"interpolate_scalar", &Interpolation::interpolate_scalar);
    register_method((char *)"interpolate", &Interpolation::interpolate);
    register_method((char *)"process", &Interpolation::process);
    register_method((char *)"is_finished", &Interpolation::is_finished);
    register_method((char *)"get_value", &Interpolation::get_value);
    register_method((char *)"process", &Interpolation::process);
    register_method((char *)"get_function", &Interpolation::get_function);
    register_method((char *)"set_function", &Interpolation::set_function);
    register_method((char *)"get_start", &Interpolation::get_start);
    register_method((char *)"set_start", &Interpolation::set_start);
    register_method((char *)"get_end", &Interpolation::get_end);
    register_method((char *)"set_end", &Interpolation::set_end);
    register_method((char *)"get_duration", &Interpolation::get_duration);
    register_method((char *)"set_duration", &Interpolation::set_duration);
    register_method((char *)"get_timestep", &Interpolation::get_timestep);
    register_method((char *)"set_timestep", &Interpolation::set_timestep);
    register_method((char *)"get_loop", &Interpolation::get_loop);
    register_method((char *)"set_loop", &Interpolation::set_loop);
    register_method((char *)"get_pingpong", &Interpolation::get_pingpong);
    register_method((char *)"set_pingpong", &Interpolation::set_pingpong);
    register_method((char *)"get_reverse", &Interpolation::get_reverse);
    register_method((char *)"set_reverse", &Interpolation::set_reverse);
}

real_t Interpolation::interpolation_factor(real_t tau, bool loop, bool pingpong, bool reverse) {
    real_t factor = tau;
    if (loop) {
	// Reset interpolation
	factor = fmod(tau, 1.0);
	if (factor < 0)
	    factor += 1.0;
    }
    // Include a reverse cycle inside 1 tau.
    if (pingpong) {
	if (factor < 0.5)
	    factor *= 2.0;
	else
	    factor = 2.0 - factor * 2.0;
    }	    
    // It may not be desired to limit the factor, maybe add another boolean argument.
    factor = (factor < 0 ? 0 : (factor > 1 ? 1 : factor));
    if (reverse)
	factor = 1.0 - factor;
    return factor;
}

/**
 * Can be called as is without the Interpolation instance.
 * @param function What type of interpolation to use.
 * @param factor Interpolation factor.
 * @param start The start value.
 * @param end The end value.
 */
real_t Interpolation::interpolate_scalar(int function, real_t factor, real_t start, real_t end) {
    switch (function) {	    
    case FUNCTION_LINEAR:
	return start + (end - start) * factor;
    case FUNCTION_SQUARE:
	return start + (end - start) * (factor * factor);
    case FUNCTION_CUBIC:
	return start + (end - start) * (factor * factor * factor);
    case FUNCTION_BEZIER:
	// FIXME!
	return start + (end - start) * sin(factor * 1.5707963267948966);
    case FUNCTION_SINE:
	return start + (end - start) * sin(factor * 1.5707963267948966);
    case FUNCTION_BOOLEAN:
	if (fmod(factor, 1.0) < 0.5)
	    return start;
	return end;
    case FUNCTION_EASE_BACK:
	// float s = 1.70158f;
	// float postFix = t/=d;
	// return c*(postFix)*t*((s+1)*t - s) + b;
	return start + (end - start) * factor * factor * (2.70158 * factor - 1.70158);
    case FUNCTION_EASE_BOUNCE:
	// if ((t/=d) < (1/2.75f)) {
	// 	return c*(7.5625f*t*t) + b;
	// } else if (t < (2/2.75f)) {
	// 	float postFix = t-=(1.5f/2.75f);
	// 	return c*(7.5625f*(postFix)*t + .75f) + b;
	// } else if (t < (2.5/2.75)) {
	// 		float postFix = t-=(2.25f/2.75f);
	// 	return c*(7.5625f*(postFix)*t + .9375f) + b;
	// } else {
	// 	float postFix = t-=(2.625f/2.75f);
	// 	return c*(7.5625f*(postFix)*t + .984375f) + b;
	// }
	break;
    case FUNCTION_EASE_CIRC:
    case FUNCTION_EASE_CUBIC:
    case FUNCTION_EASE_ELASTIC:
    case FUNCTION_EASE_EXPO:
    case FUNCTION_EASE_QUAD:
    case FUNCTION_EASE_QUART:
    case FUNCTION_EASE_QUINT:
    case FUNCTION_EASE_SINE:
    default:
	break;
    }
    return start;
}

Variant Interpolation::interpolate(int function, real_t factor, Variant start, Variant end) {
    Variant::Type t_start = start.get_type();
    Variant::Type t_end = end.get_type();
    switch (t_start) {
    case Variant::REAL:
    case Variant::INT:
    	if (t_end == Variant::REAL || t_end == Variant::INT) {
    	    real_t vstart = start;
	    real_t vend = end;
    	    real_t ret = interpolate_scalar(function, factor, vstart, vend);
	    if (t_start == Variant::INT)
		return Variant((int64_t)ret);
    	    return Variant(ret);
    	}
    	break;
    case Variant::VECTOR2:
    	if (t_end == Variant::VECTOR2 || t_end == Variant::VECTOR3) {
    	    Vector2 vstart = start;
    	    Vector2 vend = end;
    	    Vector2 ret = Vector2(interpolate_scalar(function, factor, vstart.x, vend.x), interpolate_scalar(function, factor, vstart.y, vend.y));
    	    return Variant(ret);
    	}
    	break;
    case Variant::VECTOR3:
    	if (t_end == Variant::VECTOR3 || t_end == Variant::VECTOR2) {
    	    Vector3 vstart = start;
    	    Vector3 vend = end;
    	    Vector3 ret = Vector3(interpolate_scalar(function, factor, vstart.x, vend.x), interpolate_scalar(function, factor, vstart.y, vend.y), interpolate_scalar(function, factor, vstart.z, vend.z));
    	    return Variant(ret);
    	}
    	break;
    case Variant::RECT2:
    	if (t_end == Variant::RECT2) {
    	    Rect2 vstart = start;
    	    Rect2 vend = end;
    	    Rect2 ret = Rect2(interpolate_scalar(function, factor, vstart.pos.x, vend.pos.x), interpolate_scalar(function, factor, vstart.pos.y, vend.pos.y), interpolate_scalar(function, factor, vstart.size.width, vend.size.width), interpolate_scalar(function, factor, vstart.size.height, vend.size.height));
    	    return Variant(ret);
    	}
    	break;
    // case Variant::QUAT:
    // 	if (t_end == t_start || t_end == Variant::BASIS || t_end == Variant::TRANSFORM) {
    // 	    Quat vstart = start;
    // 	    if (t_end == Variant::TRANSFORM) {
    // 		Transform t = end;
    // 		end = t.get_basis();
    // 	    }
    // 	    Quat vend = end;
    // 	    return Variant(vstart.slerp(vend, factor));
    // 	}
    // 	break;
    // case Variant::BASIS:
    // 	if (t_end == t_start || t_end == Variant::QUAT || t_end == Variant::TRANSFORM) {
    // 	    Quat vstart = start;
    // 	    if (t_end == Variant::TRANSFORM) {
    // 		Transform t = end;
    // 		end = t.get_basis();
    // 	    }
    // 	    Quat vend = end;
    // 	    return Variant(Basis(vstart.slerp(vend, factor)));
    // 	}
    // 	break;
    // case Variant::TRANSFORM:
    // 	if (t_end == t_start || t_end == Variant::QUAT || t_end == Variant::BASIS || t_end == Variant::VECTOR3) {
    // 	    Transform tstart = start;
    // 	    Quat qstart = tstart.get_basis();
    // 	    Vector3 ostart = tstart.get_origin();
    // 	    Quat qend;
    // 	    Vector3 oend;
    // 	    if (t_end == Variant::TRANSFORM) {
    // 		Transform t = end;
    // 		qend = t.get_basis();
    // 		oend = t.get_origin();
    // 	    } else if (t_end == Variant::VECTOR3) {
    // 		qend = qstart;
    // 		oend = end;
    // 	    } else {
    // 		qend = end;
    // 		oend = ostart;
    // 	    }
    // 	    Quat q = qstart.slerp(qend, factor);
    // 	    Vector3 v = ostart + (oend - ostart) * factor;
    // 	    return Variant(Transform(Basis(q), v));
    // 	}
    // 	break;
    // case Variant::TRANSFORM2D:
    // 	if (t_end == t_start || t_end == Variant::VECTOR2 || t_end == Variant::REAL || t_end == Variant::INT) {
    // 	    Transform2D tstart = start;
    // 	    double rstart = tstart.get_rotation();
    // 	    Vector2 ostart = tstart.get_origin();
    // 	    double rend;
    // 	    Vector2 oend;
    // 	    if (t_end == Variant::TRANSFORM2D) {
    // 		Transform2D t = end;
    // 		rend = t.get_rotation();
    // 		oend = t.get_origin();
    // 	    } else if (t_end == Variant::VECTOR2) {
    // 		rend = rstart;
    // 		oend = end;
    // 	    } else {
    // 		rend = end;
    // 		oend = ostart;
    // 	    }
    // 	    return Variant(Transform2D(rstart + (rend - rstart) * factor, ostart + (oend - ostart) * factor));
    // 	}
    // 	break;
    // case Variant::COLOR:
    // 	if (t_end == t_start || t_end == Variant::VECTOR3) {
    // 	    Color vstart = start;
    // 	    Color vend;
    // 	    if (t_end == Variant::VECTOR3) {
    // 		Vector3 v = end;
    // 		vend = Color(v.x, v.y, v.z, 1.0);
    // 	    } else
    // 		vend = end;
    // 	    Color ret = Color(vstart.r + (vend.r - vstart.r) * factor, vstart.g + (vend.g - vstart.g) * factor, vstart.b + (vend.b - vstart.b) * factor, vstart.a + (vend.a - vstart.a) * factor);
    // 	    return Variant(ret);
    // 	}
    // 	break;
    default:
    	break;
    }
    return Variant(0);
}

Interpolation::Interpolation() {
    _function = FUNCTION_NONE;
    _start = Variant(0.0);
    _end = Variant(1.0);
    _duration = 1.0;
    _loop = false;
    _pingpong = false;
    _reverse = false;
    _custom = NULL;
    _timestep = 0;
}

Interpolation::~Interpolation()
{
}

void Interpolation::process(real_t delta) {
    _timestep += delta;
}

bool Interpolation::is_finished(void) {
    if (!_loop && _timestep >= _duration)
	return true;
    return false;
}

Variant Interpolation::get_value(void) {
    if (_duration < 0)
	return _start;
    real_t factor = interpolation_factor(_timestep / _duration, _loop, _pingpong, _reverse);
    return interpolate(_function, factor, _start, _end);
}

int Interpolation::get_function(void)
{
    return _function;
}

void Interpolation::set_function(int function)
{
    _function = (Function)function;
}

Variant Interpolation::get_start(void)
{
    return _start;
}

void Interpolation::set_start(Variant start)
{
    _start = start;
}

Variant Interpolation::get_end(void)
{
    return _end;
}

void Interpolation::set_end(Variant end)
{
    _end = end;
}

real_t Interpolation::get_duration(void)
{
    return _duration;
}

void Interpolation::set_duration(real_t duration)
{
    _duration = (duration > 0 ? duration : 0);
}

real_t Interpolation::get_timestep(void)
{
    return _timestep;
}

void Interpolation::set_timestep(real_t timestep)
{
    _timestep = timestep;
}

bool Interpolation::get_loop(void)
{
    return _loop;
}

void Interpolation::set_loop(bool loop)
{
    _loop = loop;
}

bool Interpolation::get_pingpong(void)
{
    return _pingpong;
}

void Interpolation::set_pingpong(bool pingpong)
{
    _pingpong = pingpong;
}

bool Interpolation::get_reverse(void)
{
    return _reverse;
}

void Interpolation::set_reverse(bool reverse)
{
    _reverse = reverse;
}
