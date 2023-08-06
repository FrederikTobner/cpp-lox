#pragma once

#include <iostream>

class Value {
  public:
    enum Type {
        VAL_BOOL,
        VAL_NULL,
        VAL_NUMBER,
    };

    Value();
    Value(bool value);
    Value(double value);
    ~Value();

    [[nodiscard]] Type getType() const;
    [[nodiscard]] bool is(Type type) const noexcept;
    [[nodiscard]] bool asBool() const;
    [[nodiscard]] double asNumber() const;
    friend std::ostream & operator<<(std::ostream & os, const Value & dt);
    [[nodiscard]] bool operator==(const Value & other) const;
    [[nodiscard]] bool operator!=(const Value & other) const;
    [[nodiscard]] Value operator-() const;
    [[nodiscard]] Value operator+(const Value & other) const;
    [[nodiscard]] Value operator-(const Value & other) const;
    [[nodiscard]] Value operator*(const Value & other) const;
    [[nodiscard]] Value operator/(const Value & other) const;

  private:
    Type m_type;
    union {
        bool m_bool;
        double m_number;
    } m_underlying_value;
};