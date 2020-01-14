#pragma once

namespace PrintOnScreenHelper
{
    static void PrintOnScreenFString(const FString& text)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, text);
    }

    static void PrintOnScreenFVector(const FVector& vector)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("x : %f, y : %f, z : %f"), vector.X, vector.Y, vector.Z));
    }
}