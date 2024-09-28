#ifndef HEADER_UI
#define HEADER_UI
#include <vector>
#include <memory>

namespace DragonLib
{
    struct Element{
        /**
         * The element is expected to draw itself on the screen
         */
        virtual void draw();

        /**
         * The element is expected to check for events
         */
        virtual void poll_events();

        /**
         * The element is expected to reset any state
         */
        virtual void disable();
    };

    struct View: public Element{
        std::vector<std::unique_ptr<Element>> elements;
        bool events_enabled;
        bool visible;
        
        void draw(){
            if (!visible) return;

            for (size_t i = 0; i < elements.size(); ++i){
                elements[i]->draw();
            }
        }

        void poll_events(){
            if ((!visible) || (!events_enabled)) return;

            for (size_t i = 0; i < elements.size(); ++i){
                elements[elements.size() - i - 1]->poll_events();
            }
        }

        size_t register_element(Element* element){
            elements.emplace_back(element);
            return elements.size() - 1;
        }

        void disable(){
            for (size_t i = 0; i < elements.size(); ++i){
                elements[i]->disable();
            }
        }
    };
} // namespace DragonLib


#endif