#pragma once

#define ECC_Climbing ECC_GameTraceChannel1
#define ECC_InteractionVolume ECC_GameTraceChannel2
#define ECC_WallRunnable ECC_GameTraceChannel3
#define ECC_Bullet ECC_GameTraceChannel4
#define ECC_Melee ECC_GameTraceChannel5

const FName FXParamTraceEnd = FName("TraceEnd");

const FName SocketFPCamera = FName("CameraSocket");
const FName SocketCharacterWeapon = FName("CharacterWeaponSocket");
const FName SocketCharacterThrowable = FName("ThrowableSocket");

const FName SocketWeaponMuzzle = FName("MuzzleSocket");
const FName SocketWeaponForeGrip = FName("Grip_BoneSocket");

const FName CollisionProfilePawn = FName("Pawn");
const FName CollisionProfileNoCollision = FName("NoCollision");
const FName CollisionProfileRagdoll = FName("Ragdoll");
const FName CollisionProfilePawnInteractionVolume = FName("PawnInteractionVolume");
const FName CollisionWallRunnable = FName("WallRunnable");

const FName SectionMontageReloadEnd = FName("ReloadEnd");

const FName DebugCategoryLedgeDetection = FName("LedgeDetection");
const FName DebugCategoryCharacterAttributes = FName("CharacterAttributes");
const FName DebugCategoryRangeWeapon = FName("RangeWeapon");
const FName DebugCategoryMeleeWeapon = FName("MeleeWeapon");

const FName BB_CurrentTarget = FName("CurrentTarget");
const FName BB_NextLocation = FName("NextLocation");

UENUM(BlueprintType)
enum class EEquippedItemType : uint8
{
	None,
	Pistol,
	Rifle,
	Throwable,
	Melee
};

UENUM(BlueprintType)
enum class EAmunitionType : uint8
{
	None,
	Pistol,
	Rifle,
	ShotgunShells,
	FragGrenades,
	RifleGranades,
	MAX UMETA(Hidden)//макс кол-во амуниции
};

UENUM(BlueprintType)
enum class EEquipmentSlots : uint8
{
	None,
	SideArm,//pistol rifle
	PrimaryWeapon,
	SecondaryWeapon,
	PrimaryItemSlot,
	MeleeWeapon,
	MAX UMETA(Hidden)//макс число эелементов недоступено для выбора
};

UENUM(BlueprintType)
enum class EReticleType : uint8
{
	None,
	Default,
	SniperRifle,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMeleeAttackTypes : uint8
{
	None,
	PrimaryAttack,
	SecondaryAttack,
	MAX UMETA(Hidden)
};

enum class EAttributesType : uint8
{
	None,
	Health,
	Oxygen,
	Stamina
};

UENUM(BlueprintType)
enum class ETeams : uint8
{
	Player,
	Enemy
};