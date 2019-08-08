#include "BaseComponentInspector.h"

using namespace IonEngine;
using namespace IonEngine::Editor;



SimpleSet<BaseComponentInspector*> BaseComponentInspector::availableInspectors(16, 16);
