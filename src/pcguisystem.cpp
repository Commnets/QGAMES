#include <PacManLike/pcguisystem.hpp>

// ---
void PACMAN::GUISystem::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Vector brd (__BD 20, __BD 20, __BD 0);
	QGAMES::Vector br1 (__BD 1, __BD 1, __BD 0);
	QGAMES::Position p1 = mainWidget () -> absolutePosition () - brd;
	QGAMES::Position p2 = p1 + 
		QGAMES::Vector (__BD mainWidget () -> width (), __BD mainWidget () -> height (), __BD 0) + 
			__BD 2 * brd + __BD 5 * br1;
	s -> drawRectangle (s -> position () + QGAMES::Rectangle (p1, p2 - br1), __QGAMES_YELLOWCOLOR__, true);
	for (int i = 0; i < 3; i++)
		s -> drawRectangle (s -> position () + QGAMES::Rectangle (p1 + __BD i * br1, p2 - __BD i * br1), 
				__QGAMES_BLACKCOLOR__, false);

	QGAMES::GUISystem::drawOn (s, p);
}

// ---
PACMAN::WhenFocusDecorator::WhenFocusDecorator ()
	: QGAMES::Widget::ComplexDecorator ()
{
	addDecorator (new QGAMES::Widget::BlinkingBackgroundDecorator 
		(QGAMES::Color (__QGAMES_WHITECOLOR__), 4, 100, 200, 1));
	addDecorator (new QGAMES::Widget::RoundingDecorator 
		(QGAMES::Color (__QGAMES_WHITECOLOR__), 2, QGAMES::Color (__QGAMES_TRANSPARENTCOLOR__), 5));
}

// ---
void PACMAN::ControlWidget::processEvent (const QGAMES::Event& e)
{
	if (e.code () == __QGAMES_WIDGETGETFOCUSEVENT__)
		((QGAMES::Widget*) e.data ()) -> setDecorator (new PACMAN::WhenFocusDecorator ());
	else
	if (e.code () == __QGAMES_WIDGETLOSESFOCUSEVENT__)
		((QGAMES::Widget*) e.data ()) -> setDecorator (NULL);
		
	QGAMES::SelectionOptionsGameState::MenuControlWidget::processEvent (e);
}
