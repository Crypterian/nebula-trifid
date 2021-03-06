//------------------------------------------------------------------------------
//  havokhelloworldmain.cc
//  (C) 2013 gscept
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "havokhelloworldapplication.h"
#include "system/appentry.h"

ImplementNebulaApplication();

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const Util::CommandLineArgs& args)
{
    Tests::HavokHelloWorldApplication app;
    app.SetAppTitle("Havok Hello-world application");
    app.SetAppID("havok_helloworld");
    if (app.Open())
    {
        app.Run();
        app.Close();
	}    
	app.Exit();
}
