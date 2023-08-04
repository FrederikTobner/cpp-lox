#pragma once

class Value {
  public:
    enum Type {
        VAL_BOOL,
        VAL_NIL,
        VAL_NUMBER,
    };

    Value();
    Value(bool value);
    Value(double value);
    ~Value();

    Type getType() const;
    bool isBool() const;
    bool isNil() const;
    bool isNumber() const;
    bool asBool() const;
    double asNumber() const;
    void print() const;

  private:
    Type m_type;
    union {
        bool m_bool;
        double m_number;
    } m_underlying_value;
};