#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Common.h"
#include "CmboxModel.h"
#include "BillTypesModel.h"
#include "Model.h"
#include "ModelSummary.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setOrganizationName("");
    app.setOrganizationDomain("somecompany.com");
    app.setApplicationName("utillityBills_Ver3");

    QQmlApplicationEngine engine;

    QScopedPointer <CmboxModel> cmboxModel(new CmboxModel);
    QScopedPointer <BillTypesModel> billTypesModel(new BillTypesModel);
    QScopedPointer <Model> dataModel(new Model);
    QScopedPointer <ModelSummary> modelSummary(new ModelSummary);
    Common::setCmboxModel(&*cmboxModel);
    Common::setBillTypesModel(&*billTypesModel);
    Common::setModel(&*dataModel);
    Common::loadEverything();

    qmlRegisterSingletonType <Common> ("Common", 1,0, "Common", Common::singletonProvider);
    qmlRegisterSingletonType(QUrl("qrc:/qml/Style.qml"), "Style", 1,0, "Style");
    engine.rootContext()->setContextProperty("CmboxModel", cmboxModel.data());
    engine.rootContext()->setContextProperty("ModelSummary", modelSummary.data());
    engine.rootContext()->setContextProperty("BillTypesModel", billTypesModel.data());
    engine.rootContext()->setContextProperty("Model", dataModel.data());

    engine.load(QUrl::fromLocalFile("qml/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
