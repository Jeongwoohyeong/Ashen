#pragma once

#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, true);
#define DRAW_SPHERE_COLOR(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);
#define DRAW_SPHERE_SINGLEFRAME(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.0f);
#define DRAW_LINE(Start, End) if(GetWorld()) DrawDebugLine(this->GetWorld(), Start, End, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_LINE_SINGLEFRAME(Start, End) if(GetWorld()) DrawDebugLine(this->GetWorld(), Start, End, FColor::Red, false, -1.f, 0, 1.f);
#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(),Location, 15.0f, FColor::Yellow, true);
#define DRAW_POINT_SINGLEFRAME(Location) if(GetWorld()) DrawDebugPoint(GetWorld(),Location, 15.0f, FColor::Yellow, false, -1.0f);
#define DRAW_VECTOR(Start, End) if(GetWorld()) \
	{ \
		DrawDebugLine(this->GetWorld(), Start, End, FColor::Red, true, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), End, 15.0f, FColor::Yellow, true); \
	}
#define DRAW_VECTOR_SINGLEFRAME(Start, End) if(GetWorld()) \
	{ \
		DrawDebugLine(this->GetWorld(), Start, End, FColor::Red, false, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), End, 15.0f, FColor::Yellow, false, -1.0f); \
	}
