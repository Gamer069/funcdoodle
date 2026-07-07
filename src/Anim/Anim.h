#pragma once

namespace FuncDoodle {
	namespace Anim {
		constexpr float PI = 3.14159265358979323846;
		constexpr float C1 = 1.70158;
		constexpr float C2 = C1 * 1.525;
		constexpr float C3 = C1 + 1.0;
		constexpr float C4 = (2.0 * PI) / 3.0;
		constexpr float C5 = (2.0 * PI) / 4.5;

		/**
		 * @fn Animate
		 * @brief Animate with user-specified easing.
		 *
		 * @param forward Animation reversed?
		 * @param duration Animation duration.
		 * @param from Initial value and animation state.
		 * @param ease Animation easing.
		 *
		 * @return Value at the current time of animation.
		 */
		inline double Animate(bool forward, double duration, double& from,
			double dt, double (*ease)(double)) {
			from = std::clamp(from + (forward ? dt : -dt), 0.0, duration);
			return ease(from / duration);
		}

		// thats so springy, mate (springimate) (heheheheheheheheheheheheheh)
		/**
		 * @fn Springimate
		 *
		 * @brief Animated with spring.
		 *
		 * @param v Animation velocity. (persistent, updated).
		 * @param curernt Current animation state (updated).
		 * @param target Target animtion state to animate toward.
		 * @param stiffness Spring stiffness (higher = snappier).
		 * @param damping Damping factor (higher = less oscillation).
		 * @param dt Dt in seconds
		 *
		 * @return Value at the current time of animation.
		 */
		inline double Springimate(double& v, double& current, double target,
			double stiffness, double damping, double dt) {
			double force = (target - current) * stiffness - v * damping;
			v += force * dt;
			current += v * dt;
			return current;
		}

		// Linear
		inline double Linear(double t) {
			return t;
		}

		// Sine
		inline double InSine(double t) {
			return 1.0 - cosf(t * PI / 2.0);
		}
		inline double OutSine(double t) {
			return sinf(t * PI / 2.0);
		}
		inline double InOutSine(double t) {
			return -(cosf(PI * t) - 1.0) / 2.0;
		}

		// Quad
		inline double InQuad(double t) {
			return t * t;
		}
		inline double OutQuad(double t) {
			return 1.0 - (1.0 - t) * (1.0 - t);
		}
		inline double InOutQuad(double t) {
			return t < 0.5 ? 2.0 * t * t
						   : 1.0 - powf(-2.0 * t + 2.0, 2.0) / 2.0;
		}

		// Cubic
		inline double InCubic(double t) {
			return t * t * t;
		}
		inline double OutCubic(double t) {
			return 1.0 - powf(1.0 - t, 3.0);
		}
		inline double InOutCubic(double t) {
			return t < 0.5 ? 4.0 * t * t * t
						   : 1.0 - powf(-2.0 * t + 2.0, 3.0) / 2.0;
		}

		// Quart
		inline double InQuart(double t) {
			return t * t * t * t;
		}
		inline double OutQuart(double t) {
			return 1.0 - powf(1.0 - t, 4.0);
		}
		inline double InOutQuart(double t) {
			return t < 0.5 ? 8.0 * t * t * t * t
						   : 1.0 - powf(-2.0 * t + 2.0, 4.0) / 2.0;
		}

		// Quint
		inline double InQuint(double t) {
			return t * t * t * t * t;
		}
		inline double OutQuint(double t) {
			return 1.0 - powf(1.0 - t, 5.0);
		}
		inline double InOutQuint(double t) {
			return t < 0.5 ? 16.0 * t * t * t * t * t
						   : 1.0 - powf(-2.0 * t + 2.0, 5.0) / 2.0;
		}

		// Expo
		inline double InExpo(double t) {
			return t == 0.0 ? 0.0 : powf(2.0, 10.0 * t - 10.0);
		}
		inline double OutExpo(double t) {
			return t == 1.0 ? 1.0 : 1.0 - powf(2.0, -10.0 * t);
		}
		inline double InOutExpo(double t) {
			if (t == 0.0)
				return 0.0;
			if (t == 1.0)
				return 1.0;
			return t < 0.5 ? powf(2.0, 20.0 * t - 10.0) / 2.0
						   : (2.0 - powf(2.0, -20.0 * t + 10.0)) / 2.0;
		}

		// Circ
		inline double InCirc(double t) {
			return 1.0 - sqrtf(1.0 - t * t);
		}
		inline double OutCirc(double t) {
			return sqrtf(1.0 - powf(t - 1.0, 2.0));
		}
		inline double InOutCirc(double t) {
			return t < 0.5
					   ? (1.0 - sqrtf(1.0 - powf(2.0 * t, 2.0))) / 2.0
					   : (sqrtf(1.0 - powf(-2.0 * t + 2.0, 2.0)) + 1.0) / 2.0;
		}

		// Back
		inline double InBack(double t) {
			return C3 * t * t * t - C1 * t * t;
		}
		inline double OutBack(double t) {
			return 1.0 + C3 * powf(t - 1.0, 3.0) + C1 * powf(t - 1.0, 2.0);
		}
		inline double InOutBack(double t) {
			return t < 0.5
					   ? (powf(2.0 * t, 2.0) * ((C2 + 1.0) * 2.0 * t - C2)) /
							 2.0
					   : (powf(2.0 * t - 2.0, 2.0) *
								 ((C2 + 1.0) * (2.0 * t - 2.0) + C2) +
							 2.0) /
							 2.0;
		}

		// Elastic
		inline double InElastic(double t) {
			if (t == 0.0)
				return 0.0;
			if (t == 1.0)
				return 1.0;
			return -powf(2.0, 10.0 * t - 10.0) * sinf((t * 10.0 - 10.75) * C4);
		}
		inline double OutElastic(double t) {
			if (t == 0.0)
				return 0.0;
			if (t == 1.0)
				return 1.0;
			return powf(2.0, -10.0 * t) * sinf((t * 10.0 - 0.75) * C4) + 1.0;
		}
		inline double InOutElastic(double t) {
			if (t == 0.0)
				return 0.0;
			if (t == 1.0)
				return 1.0;
			return t < 0.5 ? -(powf(2.0, 20.0 * t - 10.0) *
								 sinf((20.0 * t - 11.125) * C5)) /
								 2.0
						   : (powf(2.0, -20.0 * t + 10.0) *
								 sinf((20.0 * t - 11.125) * C5)) /
									 2.0 +
								 1.0;
		}

		// Bounce
		inline double OutBounce(double t) {
			constexpr double n1 = 7.5625, d1 = 2.75;
			if (t < 1.0 / d1)
				return n1 * t * t;
			if (t < 2.0 / d1) {
				t -= 1.5 / d1;
				return n1 * t * t + 0.75;
			}
			if (t < 2.5 / d1) {
				t -= 2.25 / d1;
				return n1 * t * t + 0.9375;
			}
			t -= 2.625 / d1;
			return n1 * t * t + 0.984375;
		}

		inline double InBounce(double t) {
			return 1.0 - OutBounce(1.0 - t);
		}
		inline double InOutBounce(double t) {
			return t < 0.5 ? (1.0 - OutBounce(1.0 - 2.0 * t)) / 2.0
						   : (1.0 + OutBounce(2.0 * t - 1.0)) / 2.0;
		}

		// Steps
		inline double StepStart(double t) {
			return t > 0.0 ? 1.0 : 0.0;
		}
		inline double StepEnd(double t) {
			return t >= 1.0 ? 1.0 : 0.0;
		}
		inline double Steps(double t, int steps) {
			return floorf(t * (double)steps) / (double)steps;
		}

		// Smooth
		inline double SmoothStep(double t) {
			return t * t * (3.0 - 2.0 * t);
		}
		inline double SmootherStep(double t) {
			return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
		}

		// Spring (simple damped)
		inline double Spring(double t, double mass = 1.0,
			double stiffness = 100.0, double damping = 10.0) {
			double w = sqrtf(stiffness / mass);
			double d = damping / (2.0 * mass);
			if (d < w) {
				double wd = sqrtf(w * w - d * d);
				return 1.0 -
					   expf(-d * t) * (cosf(wd * t) + (d / wd) * sinf(wd * t));
			}
			return 1.0 - expf(-d * t) * (1.0 + d * t);
		}

	}  // namespace Anim
}  // namespace FuncDoodle
