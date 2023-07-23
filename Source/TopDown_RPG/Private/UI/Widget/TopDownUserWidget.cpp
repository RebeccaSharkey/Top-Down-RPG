// Copyright Spxcebxr Games


#include "UI/Widget/TopDownUserWidget.h"

void UTopDownUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
