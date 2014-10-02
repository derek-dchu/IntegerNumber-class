/**
*	IntegerNumber class for handling integer number with arbitrary length
*	Since MAX_INT = 2147483648, we can use an int to represent 0 ~ 999999999, that is every 9 digits of an integer.
*	Therefore, we set base to 1000000000.
*	
*	Version: 1.0
*	Supported Operation:
*		+, -, +=, -=, -()
*		<, >, <=, >=, == ,!=
*		abs()
*		<< (ostream)
*/

#ifndef INTEGERNUMBER_H_
#define INTEGERNUMBER_H_

#define BASE 1000000000
#define BASEDIGITS 9

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

class IntegerNumber {
private:

	void parseString(string s) {
		if (s.length() == 0) {
			throw runtime_error("Err: empty input string");
		}
		sign = 1;
		int pos = 0;

		if (s[0] == '-') {
			sign = -1;
			pos++;
		} else if (s[0] == '+') {
			pos++;
		}

		for (int i = s.length() - 1; i >= pos; i -= BASEDIGITS) {
			int x = 0;
			for (int j = max(pos, i - BASEDIGITS + 1); j <= i; j++) {
				if (s[j] < '0' || s[j] > '9') {
					throw runtime_error("Err: input contains invaild character");
				}
				x = x * 10 + s[j] - '0';
			}
			digits.push_back(x);
		}
		trim();
	}

public:
	short sign;
	vector<int> digits;

	IntegerNumber() : sign(1) {}
	IntegerNumber(string s) {
		parseString(s);
	}

	IntegerNumber(const IntegerNumber& val) {
		sign = val.sign;
		digits = val.digits;
	}

    void operator=(IntegerNumber rhs) {
    	sign = rhs.sign;
    	digits = rhs.digits;
    }	

    void operator=(string s) {
    	parseString(s);
    }
    
    void operator+=(const IntegerNumber& rhs) {
		*this = *this + rhs;
    }

    void operator-=(const IntegerNumber& rhs) {
		*this = *this - rhs;
    }

    IntegerNumber operator-() const {
    	IntegerNumber res = *this;
		res.sign = -sign;
		return res;
    }

    IntegerNumber abs() const {
		IntegerNumber res = *this;
		res.sign *= res.sign;
		return res;
	}

	// eliminate empty digits
	void trim() {
		while (!digits.empty() && !digits.back())
			digits.pop_back();
		if (digits.empty())
			sign = 1;
	}

	IntegerNumber operator+(const IntegerNumber& rhs) const {
	    if (sign == rhs.sign) {
	    	IntegerNumber res = rhs;

			for (int i = 0, carry = 0; i < (int) max(digits.size(), rhs.digits.size()) || carry; ++i) {
				if (i == (int) res.digits.size())
					res.digits.push_back(0);
				res.digits[i] += carry + (i < (int) digits.size() ? digits[i] : 0);
				carry = res.digits[i] >= BASE;
				if (carry)
					res.digits[i] -= BASE;
			}
			return res;
	    }
	    return *this - (-rhs);
	}

	IntegerNumber operator-(const IntegerNumber& rhs) const {
	    if (sign == rhs.sign) {
	    	if (abs() >= rhs.abs()) {
	    		IntegerNumber res = *this;
	    		for (int i = 0, carry = 0; i < (int) rhs.digits.size() || carry; i++) {
	    			res.digits[i] -= carry + (i < (int) rhs.digits.size() ? rhs.digits[i] : 0);
	    			carry = res.digits[i] < 0;
	    			if (carry)
	    				res.digits[i] += BASE;
	    		}
	    		res.trim();
	    		return res;
	    	}
	    	return -(rhs - *this);
	    }
	    return *this + (-rhs);
	}

	bool operator<(const IntegerNumber& rhs) const {
	    if (sign != rhs.sign)
	    	return sign < rhs.sign;
	    if (digits.size() != rhs.digits.size())
	    	return digits.size() * sign < rhs.digits.size() * rhs.sign;
	    for (int i = digits.size() - 1; i >= 0; i--) {
	    	if (digits[i] != rhs.digits[i])
	    		return digits[i] * sign < rhs.digits[i] * rhs.sign;
	    }
	    return false;
	}

	bool operator>(const IntegerNumber& rhs) const {
	    return rhs < *this;
	}

	bool operator<=(const IntegerNumber& rhs) const {
	    return !(rhs < *this);
	}

	bool operator>=(const IntegerNumber& rhs) const {
	    return !(*this < rhs);
	}

	bool operator==(const IntegerNumber& rhs) const {
		return !(rhs < *this) && !(*this < rhs);
	}

	bool operator!=(const IntegerNumber& rhs) const {
	    return (rhs < *this) || (*this < rhs);
	}

	friend ostream& operator<<(ostream& os, const IntegerNumber& val) {
		if (val.sign == -1)
			os << '-';
	    os << (val.digits.empty() ? 0 : val.digits.back());
	    for (int i = (int) val.digits.size() - 2; i >= 0; --i)
	    	os << setw(BASEDIGITS) << setfill('0') << val.digits[i];
	    return os;
	}

	friend istream& operator>>(istream& is, IntegerNumber &val) {
		string s;
		is >> s;
		val.parseString(s);
		return is;
	}
};


#endif
