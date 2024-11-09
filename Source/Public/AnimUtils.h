#pragma once

template<typename T>
T* FindNotifyByClass(UAnimSequenceBase* Animation)
{
	if (!Animation) return nullptr;

	const auto NotifyEvents = Animation->Notifies;
	for (auto NotifyEvent : NotifyEvents)
	{
		auto AnimNotify = Cast<T>(NotifyEvent.Notify);
		if (AnimNotify)
		{
			return AnimNotify;
		}
	}
	return nullptr;
};