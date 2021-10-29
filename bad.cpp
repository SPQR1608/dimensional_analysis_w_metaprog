
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

template <typename A, typename B, template <int, int> class Op>
struct Zip;

template <int ...A, int ...B, template <int, int> class Op>
struct Zip<IntList<A...>, IntList<B...>, Op>
{
    using type = IntList<Op<A, B>::value...>;
};

namespace BinOp
{
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


template<typename... Args >
class Quantity;

template<int... Args>
class Quantity<IntList<Args...>>
{
public:
    explicit Quantity(double initVal = 0) : val(initVal) {}

    double value() const { return val; }
    double& value() { return val; }

    Quantity<IntList<Args...>>& operator+=(const Quantity<IntList<Args...>>& rhs)
    {
        val += rhs.val;
        return *this;
    }

    Quantity<IntList<Args...>>& operator*=(double rhs)
    {
        val *= rhs;
        return *this;
    }

    template<int... Args2>
    const Quantity<IntList<Args2...>> operator+ (const Quantity<IntList<Args2...>>& rhs)
    {
        return Quantity<IntList<Args2...>>(val + rhs.value());
    }

    template<int... Args2>
    const Quantity<IntList<Args2...>> operator- (const Quantity<IntList<Args2...>>& rhs)
    {
        return Quantity<IntList<Args2...>>(val - rhs.value());
    }

    template<int... Args2>
    auto operator* (const Quantity<IntList<Args2...>>& rhs) const
        -> decltype(Quantity<typename Zip<IntList<Args...>, IntList<Args2...>, BinOp::Plus>::type>())
    {
        return Quantity<typename Zip<IntList<Args...>, IntList<Args2...>, BinOp::Plus>::type>(val * rhs.value());
    }

    template<int... Args2>
    auto operator/ (const Quantity<IntList<Args2...>>& rhs) const
        -> decltype(Quantity<typename Zip<IntList<Args...>, IntList<Args2...>, BinOp::Minus>::type>())
    {
        return Quantity<typename Zip<IntList<Args...>, IntList<Args2...>, BinOp::Minus>::type>(val / rhs.value());
    }

private:
    double val;
};

template<int... Args2>
const Quantity<IntList<Args2...>> operator*(double lhs, const Quantity<IntList<Args2...>>& rhs)
{
    return Quantity<IntList<Args2...>>(lhs * rhs.value());
}

template<int... Args2>
const Quantity<IntList<Args2...>> operator*(const Quantity<IntList<Args2...>>& lhs, double rhs)
{
    return Quantity<IntList<Args2...>>(rhs * lhs.value());
}

template<int... Args2>
auto operator/(double lhs, const Quantity<IntList<Args2...>>& rhs)
-> decltype(Quantity<typename Zip<Dimension<>, IntList<Args2...>, BinOp::Minus>::type>())
{
    return Quantity<typename Zip<Dimension<>, IntList<Args2...>, BinOp::Minus>::type>(lhs / rhs.value());
}

template<int... Args2>
const Quantity<IntList<Args2...>> operator/(const Quantity<IntList<Args2...>>& lhs, double rhs)
{
    return Quantity<IntList<Args2...>>(lhs.value() / rhs);
}