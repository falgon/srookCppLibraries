// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_MATH_PI_HPP
#define INCLUDED_SROOK_MATH_PI_HPP
#include <srook/config/attribute/force_inline.hpp>
#include <srook/config/cpp_predefined/feature_testing.hpp>
#include <srook/config/feature/inline_variable.hpp>
#include <srook/config/noexcept_detection.hpp>
#include <srook/math/constants/root.hpp>
#include <type_traits>

namespace srook {
namespace math {

#define DEFINE_PI_FUNCTION(NAME, VALUE)                              \
    SROOK_FORCE_INLINE static constexpr T NAME() SROOK_NOEXCEPT_TRUE \
    {                                                                \
	return T(VALUE);                                             \
    }

template <class T>
struct PI {
    static_assert(std::is_arithmetic<T>::value, "This type was not supported.");

    DEFINE_PI_FUNCTION(value, 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651L)
    DEFINE_PI_FUNCTION(half, 1.57079632679489661923132169163975144209858469968755291048747229615390820314310449931401741267105853399107404326L)
    DEFINE_PI_FUNCTION(third, 1.04719755119659774615421446109316762806572313312503527365831486410260546876206966620934494178070568932738269550L)
    DEFINE_PI_FUNCTION(two_thirds, 2.09439510239319549230842892218633525613144626625007054731662972820521093752413933241868988356141137865476539101L)
    DEFINE_PI_FUNCTION(quarter, 0.78539816339744830961566084581987572104929234984377645524373614807695410157155224965700870633552926699553702163L)
    DEFINE_PI_FUNCTION(three_quaters, 2.35619449019234492884698253745962716314787704953132936573120844423086230471465674897102611900658780098661106488L)
    DEFINE_PI_FUNCTION(mul_two, 6.28318530717958647692528676655900576839433879875021164194988918461563281257241799725606965068423413596429617303L)
    DEFINE_PI_FUNCTION(mul_four, 12.56637061435917295385057353311801153678867759750042328389977836923126562514483599451213930136846827192859234606L)
    DEFINE_PI_FUNCTION(two_div, 0.636619772367581343075535053490057448137838582961825794990669376235587190536906140360455211065012343824291370907L)
    DEFINE_PI_FUNCTION(root_two_div, 0.797884560802865355879892119868763736951717262329869315331851659341315851798603677002504667814613872860605117725L)
    DEFINE_PI_FUNCTION(root, 1.77245385090551602729816748334114518279754945612238712821380778985291128459103218137495065673854466541622682362L)
    DEFINE_PI_FUNCTION(one_div_root, 0.564189583547756286948079451560772585844050629328998856844085721710642468441493414486743660202107363443028347906L)
    DEFINE_PI_FUNCTION(root_one_div, 0.564189583547756286948079451560772585844050629328998856844085721710642468441493414486743660202107363443028347906L)
    DEFINE_PI_FUNCTION(two_div_root, 1.128379167095512573896158903121545171688101258657997713688171443421284936882986828973487320404214726886056695812L)

    SROOK_FORCE_INLINE explicit operator T() const SROOK_NOEXCEPT_TRUE
    {
	return value();
    }
};
#undef DEFINE_PI_FUNCTION

#if SROOK_CPP_VARIABLE_TEMPLATES
template <class T>
SROOK_INLINE_VARIABLE static constexpr T pi = PI<T>::value();

#define DEFINE_PI_VARIABLE_TEMPLATES(PREPOSITION) \
    template <class T>                            \
    SROOK_INLINE_VARIABLE static constexpr T PREPOSITION##_pi = PI<T>::PREPOSITION()

DEFINE_PI_VARIABLE_TEMPLATES(half);
DEFINE_PI_VARIABLE_TEMPLATES(third);
DEFINE_PI_VARIABLE_TEMPLATES(two_thirds);
DEFINE_PI_VARIABLE_TEMPLATES(quarter);
DEFINE_PI_VARIABLE_TEMPLATES(three_quaters);
DEFINE_PI_VARIABLE_TEMPLATES(mul_two);
DEFINE_PI_VARIABLE_TEMPLATES(mul_four);
DEFINE_PI_VARIABLE_TEMPLATES(two_div);
DEFINE_PI_VARIABLE_TEMPLATES(root_two_div);
DEFINE_PI_VARIABLE_TEMPLATES(root);
DEFINE_PI_VARIABLE_TEMPLATES(one_div_root);
DEFINE_PI_VARIABLE_TEMPLATES(root_one_div);
DEFINE_PI_VARIABLE_TEMPLATES(two_div_root);

#undef DEFINE_PI_VARIABLE_TEMPLATES
#endif

} // namespace math
} // namespace srook

#endif
