#include <iostream>

template<int ... Int>
struct IntList;

template<int H, int... Int>
struct IntList<H, Int...>
{
    constexpr static int Head = H;
    using Tail = IntList<Int...>;
};

template<>
struct IntList<> {};

namespace BinOp
{
    template <typename A, typename B, template <int, int> class Op>
    struct Zip;

    template <int ...A, int ...B, template <int, int> class Op>
    struct Zip<IntList<A...>, IntList<B...>, Op>
    {
        using type = IntList<Op<A, B>::value...>;
    };

    template<int a, int b>
    struct Plus
    {
        static int const value = a + b;
    };

    template<int a, int b>
    struct Minus
    {
        static int const value = a - b;
    };
}

template<int m = 0, int kg = 0, int s = 0, int A = 0, int K = 0, int mol = 0, int cd = 0>
using Dimension = IntList<m, kg, s, A, K, mol, cd>;

template<typename D>
class Quantity
{
public:
    explicit Quantity(double initVal = 0) : val(initVal) {}

    double value() const { return val; }
    double& value() { return val; }

    Quantity<D>& operator+=(const Quantity<D>& rhs)
    {
        val += rhs.val;
        return *this;
    }

    Quantity<D>& operator*=(double rhs)
    {
        val *= rhs;
        return *this;
    }

    template<typename D2>
    Quantity<D2> operator+ (const Quantity<D2>& rhs)
    {
        return Quantity<D2>(val + rhs.value());
    }

    template<typename D2>
    Quantity<D2> operator- (const Quantity<D2>& rhs)
    {
        return Quantity<IntList<Args2...>>(val - rhs.value());
    }

    template<typename D2, typename R = Quantity<typename BinOp::Zip<D, D2, BinOp::Plus>::type>>
    R operator* (const Quantity<D2>& rhs) const
    {
        return R(val * rhs.value());
    }

    template<typename D2, typename R = Quantity<typename BinOp::Zip<D, D2, BinOp::Minus>::type>>
    R operator/ (const Quantity<D2>& rhs) const
    {
        return R(val / rhs.value());
    }

private:
    double val;
};

template<typename D>
Quantity<D> operator*(double lhs, const Quantity<D>& rhs)
{
    return Quantity<D>(lhs * rhs.value());
}

template<typename D>
Quantity<D> operator*(const Quantity<D>& lhs, double rhs)
{
    return Quantity<D>(rhs * lhs.value());
}

template<typename D, typename R = Quantity<typename BinOp::Zip<Dimension<>, D, BinOp::Minus>::type>>
R operator/(double lhs, const Quantity<D>& rhs)
{
    return R(lhs / rhs.value());
}

template<typename D>
Quantity<D> operator/(const Quantity<D>& lhs, double rhs)
{
    return Quantity<D>(lhs.value() / rhs);
}

using NumberQ = Quantity<Dimension<>>;          // number without dimension
using LengthQ = Quantity<Dimension<1>>;          // meters
using MassQ = Quantity<Dimension<0, 1>>;       // kilograms
using TimeQ = Quantity<Dimension<0, 0, 1>>;    // seconds
using VelocityQ = Quantity<Dimension<1, 0, -1>>;   // meters per second
using AccelQ = Quantity<Dimension<1, 0, -2>>;   // acceleration, meters per square second
using ForceQ = Quantity<Dimension<1, 1, -2>>;   // force in newtons

int main()
{
    AccelQ    a{ 9.8 };        // acceleration of gravity
    MassQ     m{ 80 };         // 80 kg
    //// the force of gravity that acts on a body weighing 80 kg
    ForceQ    f = m * a;     // result type ForceQ

    std::cout << f.value() << std::endl;

    AccelQ a2 = 2 * a;

    using L1 = Dimension<0, 1>;
    using L2 = Dimension<1, 0, -2>;

    using L3 = BinOp::Zip<L1, L2, BinOp::Plus>::type;

    std::cout << L3::Head << std::endl;
    std::cout << L3::Tail::Head << std::endl;
    std::cout << L3::Tail::Tail::Head << std::endl;

    return 0;
}