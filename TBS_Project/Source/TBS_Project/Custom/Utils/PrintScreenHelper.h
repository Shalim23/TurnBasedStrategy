#pragma once

namespace PrintOnScreenHelper
{
    static void PrintOnScreenFString(const FString& text)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, text);
    }

    static void PrintOnScreenFVector(const FVector& vector,const FColor& color = FColor::Red)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, color, FString::Printf(TEXT("x : %f, y : %f, z : %f"), vector.X, vector.Y, vector.Z));
    }

    static void PrintOnScreenFVector(const FRotator& rotator, const FColor& color = FColor::Red)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, color, FString::Printf(TEXT("pitch : %f, yaw : %f, roll : %f"), rotator.Pitch, rotator.Yaw, rotator.Roll));
    }

    static void PrintOnScreenInt(int value)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("int : %d"), value));
    }

    static void PrintOnScreenFloat(float value)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("float : %f"), value));
    }
}