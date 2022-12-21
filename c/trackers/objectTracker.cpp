/*
 * Copyright (c) 2022, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
 
#include "objectTracker.h"

#include "objectTrackerIOU.h"
#include "objectTrackerKLT.h"


// Create
objectTracker* objectTracker::Create( objectTracker::Type type )
{
	if( type == KLT )
	{
	#if HAS_VPI
		return objectTrackerKLT::Create();
	#else
		LogError(LOG_TRACKER "couldn't create KLT tracker (not built with VPI enabled)\n");
		return NULL;
	#endif
	}
	else if( type == IOU )
	{
		return objectTrackerIOU::Create();
	}
	
	return NULL;
}


// Create
objectTracker* objectTracker::Create( const commandLine& cmdLine )
{
	const char* str = cmdLine.GetString("tracker", cmdLine.GetString("tracking"));
	const Type type = TypeFromStr(str);
	
	if( type == KLT )
	{
	#if HAS_VPI
		return objectTrackerKLT::Create(cmdLine);
	#else
		LogError(LOG_TRACKER "couldn't create KLT object tracker (not built with VPI enabled)\n");
		return NULL;
	#endif
	}
	else if( type == IOU )
	{
		return objectTrackerIOU::Create(cmdLine);
	}
	else
	{
		if( str != NULL )
			LogError(LOG_TRACKER "tried to create invalid object tracker type:  %s\n", str);
	}
	
	return NULL;
}


// Create
objectTracker* objectTracker::Create( int argc, char** argv )
{
	return Create(commandLine(argc, argv));
}


// TypeToStr
const char* objectTracker::TypeToStr( objectTracker::Type type )
{
	if( type == IOU )
		return "IOU";
	else if( type == KLT )
		return "KLT";
	else
		return "none";
}


// TypeFromStr
objectTracker::Type objectTracker::TypeFromStr( const char* str )
{
	if( !str )
		return NONE;
	
	if( strcasecmp(str, "IOU") == 0 )
		return IOU;
	else if( strcasecmp(str, "KLT") == 0 )
		return KLT;
	else 
		return NONE;
}