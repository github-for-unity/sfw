﻿#include <iterator>
#include <vector>
#include <algorithm>
#include <string>
#include <string.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "NativeInterface.h"
#include "SFWExport.h"

char* copyString(std::string orig)
{
	if(orig.length() == 0)
	{
		return nullptr;
	}

	auto orig_cstr = orig.c_str();

	if (!orig_cstr)
	{
		return nullptr;
	}

	size_t len = strlen(orig_cstr) + 1;

	auto buf = static_cast<char*>(malloc(len));
	if (buf) {
		memcpy(buf, orig_cstr, len);
	}

	return buf;
}

SFW_EXPORT_METHOD(NativeInterface*)
NativeInterface_Create(const char* path)
{
	std::string pathString = path;
	return new NativeInterface(pathString);
}

SFW_EXPORT_METHOD(char*)
NativeInterface_GetError(NativeInterface* instance)
{
	auto native_interface = static_cast<NativeInterface*>(instance);
	return copyString(native_interface->getError());
}

SFW_EXPORT_METHOD(void)
NativeInterface_GetEvents(NativeInterface* instance, SFW_Event** events, int* eventsCount)
{
	auto eventVector = instance->getEvents();
	auto count = 0;

	if (eventVector != nullptr)
	{
		auto vector = *eventVector;
		*events = new SFW_Event[vector.size()];

		for (auto iter = vector.begin(); iter != vector.end(); iter++)
		{
			auto exportEvent = *events + count;
			auto localEvent = *iter;

			exportEvent->type = localEvent->type;

			exportEvent->directory = copyString(localEvent->directory);
			exportEvent->fileA = copyString(localEvent->fileA);
			exportEvent->fileB = copyString(localEvent->fileB);

			count++;
		}
	}

	*eventsCount = count;

	delete eventVector;
}

SFW_EXPORT_METHOD(bool)
NativeInterface_HasErrored(NativeInterface* instance)
{
	auto native_interface = static_cast<NativeInterface*>(instance);
	return native_interface->hasErrored();
}

SFW_EXPORT_METHOD(bool)
NativeInterface_IsWatching(NativeInterface* instance)
{
	auto native_interface = static_cast<NativeInterface*>(instance);
	return native_interface->isWatching();
}

SFW_EXPORT_METHOD(void)
NativeInterface_Delete(NativeInterface* instance)
{
	auto native_interface = static_cast<NativeInterface*>(instance);
	delete native_interface;
}
