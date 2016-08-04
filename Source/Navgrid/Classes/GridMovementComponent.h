// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "GridMovementComponent.generated.h"

class ANavGrid;
class USplineComponent;
class USplineMeshComponent;
class UStaticMesh;
class UNavTileComponent;

/**
 * A movement component that operates on a NavGrid
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class NAVGRID_API UGridMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
	UGridMovementComponent(const FObjectInitializer &ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	/* bound to the first NavGrid found in the level */
	UPROPERTY(BlueprintReadOnly, Category = "Movement") ANavGrid *Grid = NULL;
	/* How far (in tile cost) the actor can move in one go */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement") float MovementRange = 4;
	/* How fast can the actor move */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement") float MaxSpeed = 450;
	/* Should we ignore rotation over the X axis */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement") bool LockRoll = true;
	/* Should we ignore rotation over the Y axis */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement") bool LockPitch = true;
	/* Should we ignore rotation over the Z axis */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement") bool LockYaw = false;

	/*
	Spline that is used as a path. The points are in word coords.
	
	We use ESplineCoordinateSpace::Local in the getters and setters to avoid any extra coord translation
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Visualization") USplineComponent *Spline = NULL;
	/* Mesh used to visualize the path */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Visualization") UStaticMesh *PathMesh = NULL;
	/* Vertical offset between Path and visualization */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Visualization") float VerticalOffset = 50;
	/* Distance between actor and where we start showing the path */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Visualization") float HorizontalOffset = 87.5;

	/* Create a path to Target, return false if no path is found */
	bool CreatePath(UNavTileComponent &Target);
	/* Follow an existing path */
	void FollowPath();
	/* Create a path and follow it if it exists */
	bool MoveTo(UNavTileComponent &Target);
	/* Visualize path */
	void ShowPath();
	/* Hide path */
	void HidePath();

	DECLARE_EVENT(UGridMovementComponent, FOnMovementDone);
	/* Triggered when movement ends */
	FOnMovementDone& OnMovementEnd() { return OnMovementEndEvent; }
private:
	FOnMovementDone OnMovementEndEvent;

protected:
	UPROPERTY() TArray<USplineMeshComponent *> SplineMeshes;

	/* Helper: Puts a spline mesh in the range along the spline */
	void AddSplineMesh(float From, float To);

	/* Should the actor be moved along the spline on the next Tick()? */
	bool Moving = false;
	/* How far along the spline are we */
	float Distance = 0;

	/* The tile we're currently on*/
	UNavTileComponent *Tile;

	/* Lookup from spline distance to tile */
	UNavTileComponent *GetTileAtDistance(float Distance);
	void SetTileAtInterval(UNavTileComponent *Tile, float From, float To);
	TArray<UNavTileComponent *> SplineDistanceToTile;
};
