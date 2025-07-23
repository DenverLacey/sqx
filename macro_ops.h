#ifndef _MACRO_OPS_H_
#define _MACRO_OPS_H_

#define NOTHING(...)
#define IDENTITY(x__) x__

#define DO_STRINGIFY_(x__) #x__
#define STRINGIFY(x__) DO_STRINGIFY_(x__)

#define GENSYM2_(name__, ln__) name__ ## ln__
#define GENSYM1_(name__, ln__) GENSYM2_(name__, ln__)
#define GENSYM(name__) GENSYM1_(name__, __LINE__)

#define DO_CONCAT_(a__, b__) a__ ## b__
#define CONCAT(a__, b__) DO_CONCAT_(a__, b__)

#define FOR_0_(f__, x__)
#define FOR_1_(f__, x__) f__(x__)
#define FOR_2_(f__, x__, ...)  f__(x__), FOR_1_(f__, __VA_ARGS__)
#define FOR_3_(f__, x__, ...)  f__(x__), FOR_2_(f__, __VA_ARGS__)
#define FOR_4_(f__, x__, ...)  f__(x__), FOR_3_(f__, __VA_ARGS__)
#define FOR_5_(f__, x__, ...)  f__(x__), FOR_4_(f__, __VA_ARGS__)
#define FOR_6_(f__, x__, ...)  f__(x__), FOR_5_(f__, __VA_ARGS__)
#define FOR_7_(f__, x__, ...)  f__(x__), FOR_6_(f__, __VA_ARGS__)
#define FOR_8_(f__, x__, ...)  f__(x__), FOR_7_(f__, __VA_ARGS__)
#define FOR_9_(f__, x__, ...)  f__(x__), FOR_8_(f__, __VA_ARGS__)
#define FOR_10_(f__, x__, ...) f__(x__), FOR_9_(f__, __VA_ARGS__)
#define FOR_11_(f__, x__, ...) f__(x__), FOR_10_(f__, __VA_ARGS__)
#define FOR_12_(f__, x__, ...) f__(x__), FOR_11_(f__, __VA_ARGS__)
#define FOR_13_(f__, x__, ...) f__(x__), FOR_12_(f__, __VA_ARGS__)
#define FOR_14_(f__, x__, ...) f__(x__), FOR_13_(f__, __VA_ARGS__)
#define FOR_15_(f__, x__, ...) f__(x__), FOR_14_(f__, __VA_ARGS__)
#define FOR_16_(f__, x__, ...) f__(x__), FOR_15_(f__, __VA_ARGS__)
#define FOR_17_(f__, x__, ...) f__(x__), FOR_16_(f__, __VA_ARGS__)
#define FOR_18_(f__, x__, ...) f__(x__), FOR_17_(f__, __VA_ARGS__)
#define FOR_19_(f__, x__, ...) f__(x__), FOR_18_(f__, __VA_ARGS__)
#define FOR_20_(f__, x__, ...) f__(x__), FOR_19_(f__, __VA_ARGS__)
#define FOR_21_(f__, x__, ...) f__(x__), FOR_20_(f__, __VA_ARGS__)
#define FOR_22_(f__, x__, ...) f__(x__), FOR_21_(f__, __VA_ARGS__)
#define FOR_23_(f__, x__, ...) f__(x__), FOR_22_(f__, __VA_ARGS__)
#define FOR_24_(f__, x__, ...) f__(x__), FOR_23_(f__, __VA_ARGS__)
#define FOR_25_(f__, x__, ...) f__(x__), FOR_24_(f__, __VA_ARGS__)
#define FOR_26_(f__, x__, ...) f__(x__), FOR_25_(f__, __VA_ARGS__)
#define FOR_27_(f__, x__, ...) f__(x__), FOR_26_(f__, __VA_ARGS__)
#define FOR_28_(f__, x__, ...) f__(x__), FOR_27_(f__, __VA_ARGS__)
#define FOR_29_(f__, x__, ...) f__(x__), FOR_28_(f__, __VA_ARGS__)
#define FOR_30_(f__, x__, ...) f__(x__), FOR_29_(f__, __VA_ARGS__)
#define FOR_31_(f__, x__, ...) f__(x__), FOR_30_(f__, __VA_ARGS__)
#define FOR_32_(f__, x__, ...) f__(x__), FOR_31_(f__, __VA_ARGS__)
#define FOR_33_(f__, x__, ...) f__(x__), FOR_32_(f__, __VA_ARGS__)
#define FOR_34_(f__, x__, ...) f__(x__), FOR_33_(f__, __VA_ARGS__)
#define FOR_35_(f__, x__, ...) f__(x__), FOR_34_(f__, __VA_ARGS__)
#define FOR_36_(f__, x__, ...) f__(x__), FOR_35_(f__, __VA_ARGS__)
#define FOR_37_(f__, x__, ...) f__(x__), FOR_36_(f__, __VA_ARGS__)
#define FOR_38_(f__, x__, ...) f__(x__), FOR_37_(f__, __VA_ARGS__)
#define FOR_39_(f__, x__, ...) f__(x__), FOR_38_(f__, __VA_ARGS__)
#define FOR_40_(f__, x__, ...) f__(x__), FOR_39_(f__, __VA_ARGS__)
#define FOR_41_(f__, x__, ...) f__(x__), FOR_40_(f__, __VA_ARGS__)
#define FOR_42_(f__, x__, ...) f__(x__), FOR_41_(f__, __VA_ARGS__)
#define FOR_43_(f__, x__, ...) f__(x__), FOR_42_(f__, __VA_ARGS__)
#define FOR_44_(f__, x__, ...) f__(x__), FOR_43_(f__, __VA_ARGS__)
#define FOR_45_(f__, x__, ...) f__(x__), FOR_44_(f__, __VA_ARGS__)
#define FOR_46_(f__, x__, ...) f__(x__), FOR_45_(f__, __VA_ARGS__)
#define FOR_47_(f__, x__, ...) f__(x__), FOR_46_(f__, __VA_ARGS__)
#define FOR_48_(f__, x__, ...) f__(x__), FOR_47_(f__, __VA_ARGS__)
#define FOR_49_(f__, x__, ...) f__(x__), FOR_48_(f__, __VA_ARGS__)
#define FOR_50_(f__, x__, ...) f__(x__), FOR_49_(f__, __VA_ARGS__)

#define DO_GET_FOR_MACRO_(_50__, _49__, _48__, _47__, _46__, _45__, _44__, _43__, _42__, _41__, _40__, _39__, _38__, _37__, _36__, _35__, _34__, _33__, _32__, _31__, _30__, _29__, _28__, _27__, _26__, _25__, _24__, _23__, _22__, _21__, _20__, _19__, _18__, _17__, _16__, _15__, _14__, _13__, _12__, _11__, _10__, _9__, _8__, _7__, _6__, _5__, _4__, _3__, _2__, _1__, macro__, ...) macro__
#define GET_FOR_MACRO_(...) DO_GET_FOR_MACRO_(__VA_ARGS__, FOR_50_, FOR_49_, FOR_48_, FOR_47_, FOR_46_, FOR_45_, FOR_44_, FOR_43_, FOR_42_, FOR_41_, FOR_40_, FOR_39_, FOR_38_, FOR_37_, FOR_36_, FOR_35_, FOR_34_, FOR_33_, FOR_32_, FOR_31_, FOR_30_, FOR_29_, FOR_28_, FOR_27_, FOR_26_, FOR_25_, FOR_24_, FOR_23_, FOR_22_, FOR_21_, FOR_20_, FOR_19_, FOR_18_, FOR_17_, FOR_16_, FOR_15_, FOR_14_, FOR_13_, FOR_12_, FOR_11_, FOR_10_, FOR_9_, FOR_8_, FOR_7_, FOR_6_, FOR_5_, FOR_4_, FOR_3_, FOR_2_, FOR_1_, FOR_0_)

#define FOR(f__, ...) GET_FOR_MACRO_(__VA_ARGS__)(f__, __VA_ARGS__)

#define DO_COUNT_VARARGS_(_1__, _2__, _3__, _4__, _5__, _6__, _7__, _8__, _9__, _10__, _11__, _12__, _13__, _14__, _15__, _16__, _17__, _18__, _19__, _20__, _21__, _22__, _23__, _24__, _25__, _26__, _27__, _28__, _29__, _30__, _31__, _32__, _33__, _34__, _35__, _36__, _37__, _38__, _39__, _40__, _41__, _42__, _43__, _44__, _45__, _46__, _47__, _48__, _49__, _50__, n__, ...) n__
#define COUNT_VARARGS(...) DO_COUNT_VARARGS_(__VA_ARGS__, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define FIRST_VARARGS(x__, ...) x__

#endif // _MACRO_OPS_H_

