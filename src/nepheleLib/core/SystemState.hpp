#ifndef SYSTEMSTATE_HPP
#define SYSTEMSTATE_HPP

namespace talorion {

enum class SystemState :int{
    UNKNOWN,
    INITIALIZED,
    NOT_INITIALIZED,
    STARTED,
    NOT_STARTED,
    DISPOSED,
    NOT_DISPOSED
};
}

#endif // SYSTEMSTATE_HPP
