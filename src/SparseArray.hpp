/*

*/

#ifndef SPARSEARRAY_HPP_
#define SPARSEARRAY_HPP_

#include <optional>
#include <string>
#include <vector>

template<typename Component>
class SparseArray
{
    public:
        using compRef = Component &;
        using optComponent = std::optional<Component>;
        using optCompRef = optComponent &;
        using vectArray = std::vector<optComponent>;
        using vectIndex = vectArray::size_type;
        using iterator = typename vectArray::iterator;
        using constIterator = typename vectArray::const_iterator;

    private:
        vectArray _array;

    public:
        SparseArray() = default;
        ~SparseArray() = default;

        SparseArray(const SparseArray &other) = default;
        SparseArray &operator=(const SparseArray &other) = default;

        SparseArray(SparseArray &&other) noexcept = default;
        SparseArray &operator=(SparseArray &&other) noexcept = default;

        compRef operator[](vectIndex index)
        {
            if (index >= _array.size() || index < 0) {
                throw SparseArrayException("index out of range: " + std::to_string(index));
            }
            if (!_array[index].has_value()) {
                throw SparseArrayException("index is empty: " + std::to_string(index));
            }
            return _array[index].value();
        }

#pragma region methods

        compRef get(vectIndex index) const
        {
            if (index >= _array.size() || index < 0) {
                throw SparseArrayException("index out of range: " + std::to_string(index));
            }
            if (!_array[index].has_value()) {
                throw SparseArrayException("index is empty: " + std::to_string(index));
            }
            return _array[index].value();
        }

        void set(vectIndex index, Component &&value)
        {
            if (index >= _array.size() || index < 0) {
                throw SparseArrayException("index out of range: " + std::to_string(index));
            }
            _array[index] = std::move(value);
        }

        void init(vectIndex index, optComponent value = std::nullopt)
        {
            if (index >= _array.size()) {
                _array.resize(index + 1, value);
            }
            _array[index] = value;
        }

        template<typename... Args>
        compRef emplace(vectIndex index, Args &&...args)
        {
            if (index >= _array.size()) {
                _array.resize(index + 1);
            }
            _array[index].emplace(Component(std::forward<Args>(args)...));
            return _array[index].value();
        }

        void erase(vectIndex index)
        {
            if (index >= _array.size() || index < 0) {
                throw SparseArrayException("index out of range: " + std::to_string(index));
            }
            _array[index].reset();
        }

#pragma endregion methods

#pragma region iterator

        iterator begin()
        {
            return _array.begin();
        }

        iterator end()
        {
            return _array.end();
        }

        constIterator begin() const
        {
            return _array.begin();
        }

        constIterator end() const
        {
            return _array.end();
        }

        constIterator cbegin() const
        {
            return _array.cbegin();
        }

        constIterator cend() const
        {
            return _array.cend();
        }

        vectIndex size() const
        {
            return _array.size();
        }

#pragma endregion iterator

        void clear()
        {
            _array.clear();
        }

    private:
        class SparseArrayException final : public std::exception
        {
            public:
                explicit SparseArrayException(const std::string &aMessage)
                {
                    _message += aMessage;
                }

                ~SparseArrayException() final = default;

                SparseArrayException(const SparseArrayException &other) = default;
                SparseArrayException &operator=(const SparseArrayException &other) = default;

                SparseArrayException(SparseArrayException &&other) noexcept = default;
                SparseArrayException &operator=(SparseArrayException &&other) noexcept = default;

                [[nodiscard]] const char *what() const noexcept override
                {
                    return _message.c_str();
                }

            private:
                std::string _message {"SparseArrayException: "};
        };
};

#endif /* !SPARSEARRAY_HPP_ */
