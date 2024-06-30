#include <Common/buoy.hpp>
#include <Common/element.hpp>

// ---
void* QGAMES::NotificationBuoy::treatFor (QGAMES::Element* e)
{
	e -> notify (QGAMES::Event ((int) _value, e));
	return (NULL);
}
