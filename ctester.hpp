#ifndef CTESTER_HPP
#define CTESTER_HPP
#include <stack>
#include <chrono>

class CTester {
	private:
		std::stack<std::chrono::steady_clock::time_point> m_timepts;
	public:
		void set_timer() {
			std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
			m_timepts.push(tp);
		}
		double end_timer() {
			std::chrono::steady_clock::time_point tp_e = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point tp_b = m_timepts.top();
			m_timepts.pop();
			std::chrono::duration<double> res = std::chrono::duration_cast<std::chrono::duration<double>>(tp_e - tp_b);
			return res.count();
		}
};

#endif //CTESTER_HPP
