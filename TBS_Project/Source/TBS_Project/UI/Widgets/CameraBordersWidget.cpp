#include "CameraBordersWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UCameraBordersWidget::UCameraBordersWidget(const FObjectInitializer& objectInitializer)
    : Super(objectInitializer)
{}

void UCameraBordersWidget::SetChangeStateCallback(const std::function<void(const CameraMoveState&)>& callback)
{
    m_ChangeStateCallback = callback;
}

void UCameraBordersWidget::NativeConstruct()
{
    if (Top)
    {
        Top->SetRenderOpacity(0.0f);
        Top->OnHovered.AddDynamic(this, &UCameraBordersWidget::OnTop);
        Top->OnUnhovered.AddDynamic(this, &UCameraBordersWidget::OnStateReset);
    }

    if (Bottom)
    {
        Bottom->SetRenderOpacity(0.0f);
        Bottom->OnHovered.AddDynamic(this, &UCameraBordersWidget::OnBottom);
        Bottom->OnUnhovered.AddDynamic(this, &UCameraBordersWidget::OnStateReset);
    }

    if (Left)
    {
        Left->SetRenderOpacity(0.0f);
        Left->OnHovered.AddDynamic(this, &UCameraBordersWidget::OnLeft);
        Left->OnUnhovered.AddDynamic(this, &UCameraBordersWidget::OnStateReset);
    }

    if (Right)
    {
        Right->SetRenderOpacity(0.0f);
        Right->OnHovered.AddDynamic(this, &UCameraBordersWidget::OnRight);
        Right->OnUnhovered.AddDynamic(this, &UCameraBordersWidget::OnStateReset);
    }

    if (TopLeft)
    {
        TopLeft->SetRenderOpacity(0.0f);
        TopLeft->OnHovered.AddDynamic(this, &UCameraBordersWidget::OnTopLeft);
        TopLeft->OnUnhovered.AddDynamic(this, &UCameraBordersWidget::OnStateReset);
    }

    if (TopRight)
    {
        TopRight->SetRenderOpacity(0.0f);
        TopRight->OnHovered.AddDynamic(this, &UCameraBordersWidget::OnTopRight);
        TopRight->OnUnhovered.AddDynamic(this, &UCameraBordersWidget::OnStateReset);
    }

    if (BottomLeft)
    {
        BottomLeft->SetRenderOpacity(0.0f);
        BottomLeft->OnHovered.AddDynamic(this, &UCameraBordersWidget::OnBottomLeft);
        BottomLeft->OnUnhovered.AddDynamic(this, &UCameraBordersWidget::OnStateReset);
    }

    if (BottomRight)
    {
        BottomRight->SetRenderOpacity(0.0f);
        BottomRight->OnHovered.AddDynamic(this, &UCameraBordersWidget::OnBottomRight);
        BottomRight->OnUnhovered.AddDynamic(this, &UCameraBordersWidget::OnStateReset);
    }

    AddToViewport();
}

void UCameraBordersWidget::OnStateReset()
{
    m_ChangeStateCallback(CameraMoveState::Idle);
}

void UCameraBordersWidget::OnTop()
{
    m_ChangeStateCallback(CameraMoveState::Top);
}

void UCameraBordersWidget::OnBottom()
{
    m_ChangeStateCallback(CameraMoveState::Bottom);
}

void UCameraBordersWidget::OnTopLeft()
{
    m_ChangeStateCallback(CameraMoveState::TopLeft);
}

void UCameraBordersWidget::OnLeft()
{
    m_ChangeStateCallback(CameraMoveState::Left);
}

void UCameraBordersWidget::OnRight()
{
    m_ChangeStateCallback(CameraMoveState::Right);
}

void UCameraBordersWidget::OnTopRight()
{
    m_ChangeStateCallback(CameraMoveState::TopRight);
}

void UCameraBordersWidget::OnBottomRight()
{
    m_ChangeStateCallback(CameraMoveState::BottomRight);
}

void UCameraBordersWidget::OnBottomLeft()
{
    m_ChangeStateCallback(CameraMoveState::BottomLeft);
}
