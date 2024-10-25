#ifndef HEADER_CORE_DRAGONLIB_ELEMENTS_ELEMENT
#define HEADER_CORE_DRAGONLIB_ELEMENTS_ELEMENT

namespace DragonLib
{
    namespace UI
    {
        namespace Elements
        {
            /**
             * Base class for all elements
             */
            struct Element{
            public:
                struct HoverState{
                public:
                    bool child_hovered = false; // Set to true if a child of this element is hovered
                    bool requested_pointer = false; // Set to true if the element wants the cursor to be pointer
                    bool hovered = false; // Set to true if the element is hovered

                    /** Returns true if the element is hovered (and the child is not) */
                    bool exclusively_hovered();

                    /** Resets the state to the initial condition */
                    void clear();
                } hstate;

                /**
                 * The element is expected to draw itself on the screen
                 */
                virtual void draw();

                /**
                 * The element is expected to poll for events (and set its state)
                 */
                virtual void poll_events();

                /**
                 * The element is expected to reset any state
                 */
                virtual void reset();

                virtual ~Element();

            protected:
                /**
                 * Resets the state of the element
                 */
                virtual void clear_state();
            };
        } // namespace Elements        
    } // namespace UI
} // namespace DragonLib



#endif