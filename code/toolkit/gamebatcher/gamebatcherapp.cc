//------------------------------------------------------------------------------
//  gamebatcherapp.cc
//  (C) 2011-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "gamebatcherapp.h"
#include "io/assignregistry.h"
#include "core/coreserver.h"
#include "fbx/helpers/animsplitterhelper.h"
#include "fbx/helpers/batchattributes.h"
#include "game/gameexporter.h"
#include "sqlite3/sqlite3factory.h"
#include "game/leveldbwriter.h"


using namespace IO;
using namespace Util;
using namespace ToolkitUtil;
using namespace Base;

namespace Toolkit
{
//------------------------------------------------------------------------------
/**
*/
GameBatcherApp::GameBatcherApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
GameBatcherApp::~GameBatcherApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
GameBatcherApp::Open()
{
	bool retval = DistributedToolkitApp::Open();
	return retval;
}

//------------------------------------------------------------------------------
/**
*/
void 
GameBatcherApp::DoWork()
{
   Ptr<ToolkitUtil::GameExporter> exporter = ToolkitUtil::GameExporter::Create();
   exporter->SetLogger(&this->logger);
   exporter->Open();   
   exporter->ExportAll();
   exporter->Close();
}

//------------------------------------------------------------------------------
/**
*/
bool 
GameBatcherApp::ParseCmdLineArgs()
{
	return DistributedToolkitApp::ParseCmdLineArgs();
}

//------------------------------------------------------------------------------
/**
*/
bool 
GameBatcherApp::SetupProjectInfo()
{
	if (DistributedToolkitApp::SetupProjectInfo())
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void 
GameBatcherApp::ShowHelp()
{
	n_printf("Nebula Trifid Game Batcher.\n"
		"(C) 2012-2015 Individual contributors, see AUTHORS file.\n");	
	n_printf("-help         --display this help\n"			 
			 "-dir          --category name\n"
			 "-file         --file name (if empty, dir will be parsed)\n"
			 "-projectdir   --nebula project trunk (if empty, attempts to use registry)\n"
			 "-platform     --export platform");	
}



} // namespace Toolkit

