#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <type_traits>
namespace Engine::Event {
    class Event
    {
        public:
            virtual ~Event() = default;
    };

    template<typename T>
    concept EventConcept = std::is_base_of_v<Event, T>;
} // namespace Engine::Event
#endif /* !EVENT_HPP_ */
