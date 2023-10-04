// Fill out your copyright notice in the Description page of Project Settings.
#include "HunterProjectile.h"

// Sets default values
AHunterProjectile::AHunterProjectile()
{
	SetReplicates(true);
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Comprobacion y creacion del RootComponent
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	//Comprobacion y creacion del componente de colision
	if (!CollisionComponent)
	{
		//Usar una esfera como representacion simple de colision
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

		//Definir el radio de la esfera de colision
		CollisionComponent->InitSphereRadius(15.0f);

		//Definir el componente raiz para el componente de colision
		RootComponent = CollisionComponent;
	}

	//Comprobacion y creacion del componente de movimiento del proyectil
	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	//Comprobacion y creacion del mesh del proyectil
	if(!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Models/Projectile/Sphere.Sphere'"));
		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}

	//Incluir material al proyectil
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Materials/MAT_HunterProjectile.MAT_HunterProjectile'"));
	if (Material.Succeeded())
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	//Destruir el proyectil despues de 3 segundos
	InitialLifeSpan = 3.0f;

	//Definir el perfil de colision del proyectil
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

	//Evento llamado cuando el componente golpea algo
	CollisionComponent->OnComponentHit.AddDynamic(this, &AHunterProjectile::OnHit);
}

// Called when the game starts or when spawned
void AHunterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHunterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Funcion que inicia la velocidad del proyectil en una direccion
void AHunterProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

//Funcion llamada cuando el proyectil golpea con algo
void AHunterProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);

		// ------------------ FUNCIONALIDAD SI GOLPEA A OTRO JUGADOR -------------------------
	}

	Destroy();
}
