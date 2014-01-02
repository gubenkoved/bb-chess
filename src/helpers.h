#ifndef HELPERS_H
#define HELPERS_H

#include <QList>

#include "figureposition.h"

class Helpers
{
public:
    template<typename Container, typename T>
        static inline void AppendIfNotNull(Container& container, const T& value);

    template<typename Container, typename T>
        static inline void AppendIfNotExists(Container& container, const T& value);

    template<typename T, template<typename T> class Container>
        static inline void AppendIfNotExists(Container<T>& container, const Container<T>& values);

    template<typename Container>
        static inline void AppendIfValid(Container& container, const POSITION& position);

    template<typename T, template<typename T> class Container>
        static inline void AppendIfValid(Container<T>& target, const Container<T>& source);

    //static void AppendIfValidAndNotExists(PositionList& list, const POSITION& position);
    //static void AppendIfValidAndNotExists(PositionList& list, const PositionList& positions);
};

template<typename Container, typename T>
    void Helpers::AppendIfNotNull(Container& container, const T& value)
{
    if (value != NULL)
    {
        container.append(value);
    }
}

template<typename Container, typename T>
    void Helpers::AppendIfNotExists(Container& container, const T& value)
{
    if (!container.contains(value))
    {
        container.append(value);
    }
}

template<typename T, template<typename T> class Container>
    void Helpers::AppendIfNotExists(Container<T>& container, const Container<T>& values)
{
    foreach(T value, values)
    {
        AppendIfNotExists(container, value);
    }
}

template<typename Container>
    void Helpers::AppendIfValid(Container& container, const POSITION& position)
{
    if (!PositionHelper::IsInvalid(position))
    {
        container.append(position);
    }
}

template<typename T, template<typename T> class Container>
    void Helpers::AppendIfValid(Container<T>& target, const Container<T>& source)
{
    foreach(POSITION p, source)
    {
        AppendIfValid(target, p);
    }
}

#endif // HELPERS_H
