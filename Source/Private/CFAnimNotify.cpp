// Fill out your copyright notice in the Description page of Project Settings.


#include "CFAnimNotify.h"

void UCFAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
   OnNotified.Broadcast(MeshComp);
   Super::Notify(MeshComp, Animation);
}