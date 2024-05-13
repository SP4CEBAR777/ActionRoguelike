// Fill out your copyright notice in the Description page of Project Settings.

#include "SActionComponent.h"
#include "../ActionRogueLike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "SAction.h"

USActionComponent::USActionComponent() {
  PrimaryComponentTick.bCanEverTick = true;

  SetIsReplicatedByDefault(true);
}

void USActionComponent::BeginPlay() {
  Super::BeginPlay();

  // Server Only
  if (GetOwner()->HasAuthority()) {
    for (TSubclassOf<USAction> ActionClass : DefaultActions) {
      AddAction(GetOwner(), ActionClass);
    }
  }
}

void USActionComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // FString DebugMsg = GetNameSafe(GetOwner()) + " : " +
  // ActiveGameplayTags.ToStringSimple(); GEngine->AddOnScreenDebugMessage(-1,
  // 0.0f, FColor::White, DebugMsg);

  // for (USAction *Action : Actions) {
  //   FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

  // FString ActionMsg =
  //     FString::Printf(TEXT("[%s] Action: %s : "), *GetNameSafe(GetOwner()),
  //                     *GetNameSafe(Action));

  // LogOnScreen(this, ActionMsg, TextColor, 0.0f);
}

void USActionComponent::AddAction(AActor *Instigator,
                                  TSubclassOf<USAction> ActionClass) {
  if (!ensure(ActionClass)) {
    return;
  }

  USAction *NewAction = NewObject<USAction>(GetOwner(), ActionClass);
  if (ensure(NewAction)) {
    NewAction->Initialize(this);
    Actions.Add(NewAction);
  }

  if (NewAction->bIsAutoStart && ensure(NewAction->CanStart(Instigator))) {
    NewAction->StartAction(Instigator);
  }
}

void USActionComponent::RemoveAction(USAction *ActionClass) {
  if (!ensure(ActionClass && !ActionClass->IsRunning())) {
    return;
  }

  Actions.Remove(ActionClass);
}

bool USActionComponent::StartActionByName(AActor *Instigator,
                                          FName ActionName) {
  for (USAction *Action : Actions) {
    if (Action && Action->ActionName == ActionName) {
      if (!Action->CanStart(Instigator)) {
        FString FailedMsg =
            FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
        continue;
      }

      // Is Client
      if (!GetOwner()->HasAuthority()) {
        ServerStartActionByName(Instigator, ActionName);
      }

      Action->StartAction(Instigator);
      return true;
    }
  }
  return false;
}

void USActionComponent::ServerStartActionByName_Implementation(
    AActor *Instigator, FName ActionName) {
  StartActionByName(Instigator, ActionName);
}

bool USActionComponent::StopActionByName(AActor *Instigator, FName ActionName) {
  for (USAction *Action : Actions) {
    if (Action && Action->ActionName == ActionName) {
      if (Action->IsRunning()) {

        // Is Client
        if (!GetOwner()->HasAuthority()) {
          ServerStopActionByName(Instigator, ActionName);
        }

        Action->StopAction(Instigator);
        return true;
      }
    }
  }
  return false;
}

void USActionComponent::ServerStopActionByName_Implementation(
    AActor *Instigator, FName ActionName) {
  StopActionByName(Instigator, ActionName);
}

USAction *
USActionComponent::GetAction(TSubclassOf<USAction> ActionToFind) const {
  for (USAction *Action : Actions) {
    if (Action && Action->IsA(ActionToFind)) {
      return Action;
    }
  }
  return nullptr;
}

bool USActionComponent::ReplicateSubobjects(UActorChannel *Channel,
                                            FOutBunch *Bunch,
                                            FReplicationFlags *RepFlags) {
  bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
  for (USAction *Action : Actions) {
    if (Action) {
      WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
    }
  }

  return WroteSomething;
}

void USActionComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(USActionComponent, Actions);
}
