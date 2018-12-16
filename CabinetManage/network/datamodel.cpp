#include "datamodel.h"

#include <QApplication>
#include <QPixmap>
#include <QBuffer>

using namespace DataModel;

QString DataModel::uuid()
{
    QString uuid = QUuid::createUuid().toString();
    uuid.replace("{", "");//删除大括号
    uuid.replace("}", "");
    return uuid;
}

QByteArray DataModel::getPhotoBase64(QString path)
{
    QPixmap pixmap(path);
    QByteArray bytes;
    QBuffer buffer(&bytes);
    if( buffer.open(QIODevice::WriteOnly) ){
        pixmap.save(&buffer, "JPG");
        buffer.close();
    }
    return bytes.toBase64();
}

/**
 ********************************************************************
 * 枚举:
 ********************************************************************
*/

//警衔
QString DataModel::rank(int rank)
{
    switch ((Rank)rank) {
    case Lv0:   return QObject::tr("Lv0");
    case Lv1:   return QObject::tr("Lv1");
    case Lv2:   return QObject::tr("Lv2");
    case Lv3:   return QObject::tr("Lv3");
    case Lv4:   return QObject::tr("Lv4");
    case Lv5:   return QObject::tr("Lv5");
    case Lv6:   return QObject::tr("Lv6");
    case Lv7:   return QObject::tr("Lv7");
    case Lv8:   return QObject::tr("Lv8");
    case Lv9:   return QObject::tr("Lv9");
    case Lv10:  return QObject::tr("Lv10");
    case Lv11:  return QObject::tr("Lv11");
    default:    break;
    }
    return QObject::tr("Data error");
}

//政治面貌
QString DataModel::departy(int departy)
{
    switch ((Departy)departy) {
    case CPCMember: return QObject::tr("CPCMember");
    case CPCYouth:  return QObject::tr("CPCYouth");
    case Masses:    return QObject::tr("Masses");
    default: break;
    }
    return QObject::tr("Data error");
}

//枪支状态
QString DataModel::objectStatus(const ObjectStatus &status)
{
    switch(status){
    case InLibrary:         return  QObject::tr("InLibrary");       // 正常在库
    case PoliceOut:         return  QObject::tr("PoliceOut");       // 出警领出
    case MaintainOut:       return  QObject::tr("MaintainOut");     // 保养领出
    case ApplyStorage:      return  QObject::tr("ApplyStorage");    // 申请入库
    case ApplyScrap:        return  QObject::tr("ApplyScrap");      // 申请报废
    case ApplyTemporary:    return  QObject::tr("ApplyTemporary");  // 申请临时存放
    case TemporaryStorage:  return  QObject::tr("TemporaryStorage");// 临时存放
    case Abnormal:          return  QObject::tr("Abnormal");        // 异常不在位
    }
    return QObject::tr("Data error");
}

//判断枪支是否在库
bool DataModel::isInLibrary(const ObjectStatus &status)
{
    switch (status) {
    case InLibrary:         return true;
    case PoliceOut:         return false;
    case MaintainOut:       return false;
    case ApplyStorage:      return false;
    case ApplyScrap:        return true;
    case ApplyTemporary:    return false;
    case TemporaryStorage:  return false;
    case Abnormal:          return false;
    default: break;
    }
    return false;
}

//任务类型
QString DataModel::taskTypeName(int type)
{
    switch (type) {
    case 1:     return QObject::tr("Emergency");    //紧急出警
    case 2:     return QObject::tr("OutPolice");    //出警
    case 3:     return QObject::tr("Keep");         //保养
    case 4:     return QObject::tr("ApplyStorage"); //申请入库
    case 5:     return QObject::tr("ApplyScrap");   //申请报废
    case 6:     return QObject::tr("TempDeposit");  //临时存放
    case 8133:  return QObject::tr("Other");
    default:    break;
    }
    return QObject::tr("Data error");
}

//任务子类型
QString DataModel::taskSubType(int type)
{
    switch ((TaskSubType)type) {
    case OtherTaskSubType:  return QObject::tr("Other");    //其他
    case Obscenity:         return QObject::tr("Obscenity");//涉黄
    case Gamble:            return QObject::tr("Gamble");   //涉赌
    case Narcotics:         return QObject::tr("Narcotics");//涉毒
    case Penal:             return QObject::tr("Penal");    //刑事案件
    default:    break;
    }
    return QObject::tr("Data error");
}

//任务状态
QString DataModel::taskStatus(int status)
{
    switch ((TaskStatus)status) {
    case Applying:      return QObject::tr("applying");             //申请中
    case NotApproved:   return QObject::tr("Not approved");         //未审批
    case Approved:      return QObject::tr("Approved");             //已审批
    case NotPass:       return QObject::tr("Approval does not pass");//审批不通过
    case Executing:     return QObject::tr("Executing");            //执行中
    case End:           return QObject::tr("End");                  //结束
    default: break;
    }
    return QObject::tr("Data error");
}

//任务清单状态
QString DataModel::taskItemStatus(TaskItemStatus state)
{
    switch (state) {
    case Unexecuted:    return QObject::tr("Receiving..."); //领取中...
    case Receiving:     return QObject::tr("Executing..."); //执行中...
    case Returning:     return QObject::tr("Returning..."); //归还中...
    case Finished:      return QObject::tr("Complete");     //清单完成
    default:    break;
    }
    return QObject::tr("Data error");
}

QString DataModel::taskItemStatus(TaskItemStatus state, TaskType type)
{
    QString text;
    if (type == Storage || type == TempDeposit) {
        switch (state) {
        case Unexecuted:    return QObject::tr("Storaging..."); // 存放中...
        case Receiving:     return QObject::tr("Storaged");     // 已存
        case Returning:     return QObject::tr("Receiving..."); // 领取中...
        case Finished:      return QObject::tr("Complete");     // 清单完成
        default: break;
        }
    }
    else {
        text = taskItemStatus(state);
    }
    return text;
}

// 根据 Task Item Status 判断是否是要领取设备
bool DataModel::isGetObject(TaskItemStatus status)
{
    switch (status) {
    case Unexecuted:
    case Receiving:     return true;
    case Returning:
    case Finished:      return false;
    default: break;
    }
    return true;
}

//警员身份类型
QString DataModel::policeType(int type)
{
    switch (type) {
    case 0:     return QObject::tr("Administrators");   //超级管理员
    case 1:     return QObject::tr("Gun manager");      //枪械管理员
    case 2:     return QObject::tr("Police");           //普通警员
    case 3:     return QObject::tr("Leader");           //领导
    default:    break;
    }
    return QObject::tr("Data error");
}

//枪柜类型
QString DataModel::cabinetType(int type)
{
    switch (type) {
    case 1:     return QObject::tr("Gun cabinet");      //枪支柜
    case 2:     return QObject::tr("Ammo cabinet");     //弹药柜
    case 3:     return QObject::tr("Mixing cabinet");   //枪支弹药柜（混合柜）
    default:    break;
    }
    return QObject::tr("Data error");
}



QIcon DataModel::typePhoto(int typeNo)
{
    QString root = QApplication::applicationDirPath();//应用程序路径
    return QIcon(root+"/image/objecttype/"+QString::number(typeNo)+".png");
}





/* 解壳 "{"Success":true,"Body":1455107026429}" */
QtJson::JsonObject DataModel::doDeShell(QString json)
{
    QtJson::JsonObject jsonData = QtJson::parse(json).toMap();

    return jsonData["Body"].toMap();
}

QtJson::JsonArray DataModel::doDeListShell(QString json)
{
    QtJson::JsonObject jsonData = QtJson::parse(json).toMap();

    return jsonData["Body"].toList();
}

//加壳
QByteArray DataModel::addShell(QByteArray json)
{
    return "{ \"Success\" : true, \"Body\" : " + json + " }";
}

/*"{"Message":"未找到与请求 URI“...”匹配的 HTTP 资源。"}" */
bool DataModel::doDeSerialize(QString json, QString &msg)
{
    QtJson::JsonObject jsonData = QtJson::parse(json).toMap();
    if(jsonData.isEmpty()) return false;
    msg = jsonData["Message"].toString();
    return true;
}

/**
 ********************************************************************
 * 反馈信息:
 ********************************************************************
*/

void DataModel::initModel(Message &msg)
{
    msg.Success = false;
    msg.Body.clear();
}

bool DataModel::doDeSerialize(QString json, Message &msg)
{
    return doDeSerialize(QtJson::parse(json).toMap(), msg);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, Message &msg)
{
    if(jsonData.isEmpty()) return false;
    msg.Success = jsonData["Success"].toBool();
    msg.Body    = jsonData["Body"].toString();
    return msg.Success;
}

QByteArray DataModel::serialize(Message &msg)
{
    QtJson::JsonObject jsonData;
    jsonData["Success"] = msg.Success;
    jsonData["Body"]    = msg.Body;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 时间:
 ********************************************************************
*/

void DataModel::initModel(Time &time)
{
    time.Success = false;
    time.Body = 0;
}

bool DataModel::doDeSerialize(QString json, Time &time)
{
    return doDeSerialize(QtJson::parse(json).toMap(), time);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, Time &time)
{
    if(jsonData.isEmpty()) return false;
    time.Success    = jsonData["Success"].toBool();
    time.Body       = jsonData["Body"].toULongLong();
    return time.Success;
}

QByteArray DataModel::serialize(Time &time)
{
    QtJson::JsonObject jsonData;
    jsonData["Success"]   = time.Success;
    jsonData["Body"]      = time.Body;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 警员登录模型:
 ********************************************************************
*/

void DataModel::initModel(Login &model)
{
    model.No.clear();
    model.Password.clear();
}

bool DataModel::doDeSerialize(QString json, Login &model)
{
    return doDeSerialize(QtJson::parse(json).toMap(), model);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, Login &model)
{
    if(jsonData.isEmpty()) return false;
    model.No        = jsonData["No"].toString();
    model.Password  = jsonData["Body"].toString();
    return true;
}

QByteArray DataModel::serialize(Login &model)
{
    QtJson::JsonObject jsonData;
    jsonData["No"]      = model.No;
    jsonData["Password"]= model.Password;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 设置值班领导/枪管员模型:
 ********************************************************************
*/

void DataModel::initModel(SetCurrent &model)
{
    model.RoomId.clear();
    model.CurrentId.clear();
    model.NewId.clear();
}

bool DataModel::doDeSerialize(QString json, SetCurrent &model)
{
    return doDeSerialize(QtJson::parse(json).toMap(), model);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, SetCurrent &model)
{
    if(jsonData.isEmpty()) return false;
    model.RoomId    = jsonData["RoomId"].toString();
    model.CurrentId = jsonData["CurrentId"].toString();
    model.NewId     = jsonData["NewId"].toString();
    return true;
}

QByteArray DataModel::serialize(SetCurrent &model)
{
    QtJson::JsonObject jsonData;
    jsonData["RoomId"]      = model.RoomId;
    jsonData["CurrentId"]   = model.CurrentId;
    jsonData["NewId"]       = model.NewId;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 枪支弹药类型:
 ********************************************************************
*/

void DataModel::initModel(ObjectType &item)
{
    item.No = 0;
    item.Name.clear();
    item.AmmoTypeNo = 0;
    item.AmmoBoxTypeNo = 0;
    item.AmmoNumber = 0;
    item.AmmoBoxNumber = 0;
}

bool DataModel::doDeSerialize(QString json, ObjectType &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, ObjectType &item)
{
    if(jsonData.isEmpty()) return false;
    item.No             = jsonData["No"].toInt();
    item.Name           = jsonData["Name"].toString();
    item.AmmoTypeNo     = jsonData["AmmoTypeNo"].toInt();
    item.AmmoBoxTypeNo  = jsonData["AmmoBoxTypeNo"].toInt();
    item.AmmoNumber     = jsonData["AmmoNumber"].toInt();
    item.AmmoBoxNumber  = jsonData["AmmoBoxNumber"].toInt();
    return true;
}

QByteArray DataModel::serialize(ObjectType &item)
{
    QtJson::JsonObject jsonData;
    jsonData["No"]              = item.No;
    jsonData["Name"]            = item.Name;
    jsonData["AmmoTypeNo"]      = item.AmmoTypeNo;
    jsonData["AmmoBoxTypeNo"]   = item.AmmoBoxTypeNo;
    jsonData["AmmoNumber"]      = item.AmmoNumber;
    jsonData["AmmoBoxNumber"]   = item.AmmoBoxNumber;
    return QtJson::serialize(jsonData);
}

void DataModel::initModel(ObjectTypeList &list)
{
    list.clear();
}

static bool sortObjectType(const DataModel::ObjectType &first, const DataModel::ObjectType &second)
{
    return (first.No < second.No);
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, ObjectTypeList &list)
{
    if(array.isEmpty()) return false;
    ObjectType item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    qSort(list.begin(), list.end(), sortObjectType);
    return true;
}

//根据类型ID，获取类型名称
ObjectType DataModel::objectType(const ObjectTypeList &typeList,int typeId)
{
    for( int j = 0; j < typeList.length(); j++ )
    {
        if(typeList[j].No == typeId) return typeList[j];
    }
    return ObjectType();
}



/**
 ********************************************************************
 * 弹药模型:
 ********************************************************************
*/

void DataModel::initModel(Ammo &item)
{
    item.Id.clear();
    item.RoomId.clear();
    item.RoomName.clear();
    item.CabId.clear();
    item.CabNo.clear();
    item.SubCabId.clear();
    item.SubCabNo.clear();
    item.Weight = 0;
    item.ObjectNumber = 0;
    item.ObjectStatus = Abnormal;
    item.ObjectTypeId = 0;
    item.IsBox = false;
    item.TaskId.clear();
    item.AddTime = 0;
}

bool DataModel::doDeSerialize(QString json, Ammo &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, Ammo &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id             = jsonData["Id"].toString();
    item.RoomId         = jsonData["RoomId"].toString();
    item.RoomName       = jsonData["RoomName"].toString();
    item.CabId          = jsonData["CabId"].toString();
    item.CabNo          = jsonData["CabNo"].toString();
    item.SubCabId       = jsonData["SubCabId"].toString();
    item.SubCabNo       = jsonData["SubCabNo"].toString();
    item.Weight         = jsonData["Weight"].toInt();
    item.ObjectNumber   = jsonData["ObjectNumber"].toInt();
    item.ObjectStatus   = ObjectStatus(jsonData["ObjectStatus"].toInt());
    item.ObjectTypeId   = jsonData["ObjectTypeId"].toInt();
    item.IsBox          = jsonData["IsBox"].toBool();
    item.TaskId         = jsonData["TaskId"].toString();
    item.AddTime        = jsonData["AddTime"].toULongLong();
    return true;
}

QByteArray DataModel::serialize(Ammo &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]          = item.Id;
    jsonData["RoomId"]      = item.RoomId;
    jsonData["RoomName"]    = item.RoomName;
    jsonData["CabId"]       = item.CabId;
    jsonData["CabNo"]       = item.CabNo;
    jsonData["SubCabId"]    = item.SubCabId;
    jsonData["SubCabNo"]    = item.SubCabNo;
    jsonData["Weight"]      = item.Weight;
    jsonData["ObjectNumber"]= item.ObjectNumber;
    jsonData["ObjectStatus"]= item.ObjectStatus;
    jsonData["ObjectTypeId"]= item.ObjectTypeId;
    jsonData["IsBox"]       = item.IsBox;
    jsonData["TaskId"]      = item.TaskId;
    jsonData["AddTime"]     = item.AddTime;
    return QtJson::serialize(jsonData);
}

void DataModel::initModel(AmmoList &list)
{
    list.clear();
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, AmmoList &list)
{
    if(array.isEmpty()) return false;
    Ammo item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    return true;
}



/**
 ********************************************************************
 * 枪支模型:
 ********************************************************************
*/

void DataModel::initModel(Gun &item)
{
    item.Id.clear();
    item.RoomId.clear();
    item.RoomName.clear();
    item.CabId.clear();
    item.CabNo.clear();
    item.SubCabId.clear();
    item.SubCabNo.clear();
    item.No.clear();
    item.Eno.clear();
    item.ObjectStatus = Abnormal;
    item.ObjectTypeId = 0;
    item.PoliceNo.clear();
    item.TaskId.clear();
    item.AddTime = 0;
}

bool DataModel::doDeSerialize(QString json, Gun &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, Gun &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id            = jsonData["Id"].toString();
    item.RoomId        = jsonData["RoomId"].toString();
    item.RoomName      = jsonData["RoomName"].toString();
    item.CabId         = jsonData["CabId"].toString();
    item.CabNo         = jsonData["CabNo"].toString();
    item.SubCabId      = jsonData["SubCabId"].toString();
    item.SubCabNo      = jsonData["SubCabNo"].toString();
    item.No            = jsonData["No"].toString();
    item.Eno           = jsonData["Eno"].toString();
    item.ObjectStatus  = ObjectStatus(jsonData["ObjectStatus"].toInt());
    item.ObjectTypeId  = jsonData["ObjectTypeId"].toInt();
    item.PoliceNo      = jsonData["PoliceNo"].toString();
    item.TaskId        = jsonData["TaskId"].toString();
    item.AddTime       = jsonData["AddTime"].toULongLong();
    return true;
}

QByteArray DataModel::serialize(Gun &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]          = item.Id;
    jsonData["RoomId"]      = item.RoomId;
    jsonData["RoomName"]    = item.RoomName;
    jsonData["CabId"]       = item.CabId;
    jsonData["CabNo"]       = item.CabNo;
    jsonData["SubCabId"]    = item.SubCabId;
    jsonData["SubCabNo"]    = item.SubCabNo;
    jsonData["No"]          = item.No;
    jsonData["Eno"]         = item.Eno;
    jsonData["ObjectStatus"]= item.ObjectStatus;
    jsonData["ObjectTypeId"]= item.ObjectTypeId;
    jsonData["PoliceNo"]    = item.PoliceNo;
    jsonData["TaskId"]      = item.TaskId;
    jsonData["AddTime"]     = item.AddTime;
    return QtJson::serialize(jsonData);
}

void DataModel::initModel(GunList &list)
{
    list.clear();
}

static bool sortGun(const DataModel::Gun &first, const DataModel::Gun &second)
{
    return (first.No < second.No);
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, GunList &list)
{
    if(array.isEmpty()) return false;
    Gun item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    qSort(list.begin(), list.end(), sortGun);
    return true;
}



/**
 ********************************************************************
 * 枪支心跳包模型:
 ********************************************************************
*/

void DataModel::initModel(GunHeart &item)
{
    item.GunId.clear();
    item.Status = 0;
}

bool DataModel::doDeSerialize(QString json, GunHeart &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, GunHeart &item)
{
    if(jsonData.isEmpty()) return false;
    item.GunId  = jsonData["GunId"].toString();
    item.Status = jsonData["Status"].toInt();
    return true;
}

QByteArray DataModel::serialize(GunHeart &item)
{
    QtJson::JsonObject jsonData;
    jsonData["GunId"]   = item.GunId;
    jsonData["Status"]  = item.Status;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 枪弹子柜模型:
 ********************************************************************
*/

void DataModel::initModel(SubCabinet &item)
{
    item.Id.clear();
    item.RoomId.clear();
    item.RoomName.clear();
    item.CabId.clear();
    item.CabNo.clear();
    item.No        = 0;
    item.isChecked = false;
    item.number    = 0;
    item.SubCabType= 0;
    item.NetAddress.clear();
    item.IpAddress.clear();
    item.Eno.clear();
    item.AddTime   = 0;
    item.Ammos.clear();
    item.Guns.clear();
}

bool DataModel::doDeSerialize(QString json, SubCabinet &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, SubCabinet &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id        = jsonData["Id"].toString();
    item.RoomId    = jsonData["RoomId"].toString();
    item.RoomName  = jsonData["RoomName"].toString();
    item.CabId     = jsonData["CabId"].toString();
    item.CabNo     = jsonData["CabNo"].toString();
    item.No        = jsonData["No"].toInt();
    item.isChecked = false;//jsonData["isChecked"].toBool();
    item.number    = 0;//jsonData["number"].toInt();
    item.SubCabType= jsonData["SubCabType"].toInt();
    item.NetAddress= jsonData["NetAddress"].toString();
    item.IpAddress = jsonData["IpAddress"].toString();
    item.Eno       = jsonData["Eno"].toString();
    item.AddTime   = jsonData["AddTime"].toULongLong();
    doDeSerialize(jsonData["Ammos"].toList(), item.Ammos);//反序列化 弹药列表
    doDeSerialize(jsonData["Guns"].toList(), item.Guns);//反序列化 枪支列表
    return true;
}

QByteArray DataModel::serialize(SubCabinet &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]          = item.Id;
    jsonData["RoomId"]      = item.RoomId;
    jsonData["RoomName"]    = item.RoomName;
    jsonData["CabId"]       = item.CabId;
    jsonData["CabNo"]       = item.CabNo;
    jsonData["No"]          = item.No;
    //jsonData["isChecked"]   = item.isChecked;
    //jsonData["number"]      = item.num;
    jsonData["SubCabType"]  = item.SubCabType;
    jsonData["NetAddress"]  = item.NetAddress;
    jsonData["IpAddress"]   = item.IpAddress;
    jsonData["Eno"]         = item.Eno;
    jsonData["AddTime"]     = item.AddTime;
    QtJson::JsonArray array;

    for(int i=0; i < item.Ammos.length(); i++){
        array << QtJson::parse(serialize(item.Ammos[i]));
    }
    jsonData["Ammos"] = array;

    array.clear();
    for(int i=0; i < item.Guns.length(); i++){
        array << QtJson::parse(serialize(item.Guns[i]));
    }
    jsonData["Guns"] = array;

    return QtJson::serialize(jsonData);
}

void DataModel::initModel(SubCabinetList &list)
{
    list.clear();
}

static bool sortSubCabinet(const DataModel::SubCabinet &first, const DataModel::SubCabinet &second)
{
    return (first.No < second.No);
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, SubCabinetList &list)
{
    if(array.isEmpty()) return false;
    SubCabinet item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    qSort(list.begin(), list.end(), sortSubCabinet);
    return true;
}

QByteArray DataModel::serialize(SubCabinetList list)
{
    QtJson::JsonObject jsonData;
    QtJson::JsonArray array;

    for(int i = 0; i < list.length(); i++){
        array << QtJson::parse(serialize(list[i]));
    }
    jsonData["Success"] = true;
    jsonData["Body"] = array;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 枪弹柜模型:
 ********************************************************************
*/

void DataModel::initModel(Cabinet &item)
{
    item.Id.clear();
    item.RoomId.clear();
    item.RoomName.clear();
    item.No.clear();
    item.CabType   = 0;
    item.NetAddress.clear();
    item.IpAddress.clear();
    item.LordId.clear();
    item.NetStatus = 0;
    item.EleStatus = 0;
    item.TempStatus= 0;
    item.AddTime   = 0;
    item.SubCabs.clear();
}

bool DataModel::doDeSerialize(QString json, Cabinet &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, Cabinet &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id         = jsonData["Id"].toString();
    item.RoomId     = jsonData["RoomId"].toString();
    item.RoomName   = jsonData["RoomName"].toString();
    item.No         = jsonData["No"].toString();
    item.CabType    = jsonData["CabType"].toInt();
    item.NetAddress = jsonData["NetAddress"].toString();
    item.IpAddress  = jsonData["IpAddress"].toString();
    item.LordId     = jsonData["LordId"].toString();
    item.NetStatus  = jsonData["NetStatus"].toInt();
    item.EleStatus  = jsonData["EleStatus"].toInt();
    item.TempStatus = jsonData["TempStatus"].toInt();
    item.AddTime    = jsonData["AddTime"].toULongLong();
    doDeSerialize(jsonData["SubCabs"].toList(), item.SubCabs);//反序列化 子柜列表
    return true;
}

QByteArray DataModel::serialize(Cabinet &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]          = item.Id;
    jsonData["RoomId"]      = item.RoomId;
    jsonData["RoomName"]    = item.RoomName;
    jsonData["No"]          = item.No;
    jsonData["CabType"]     = item.CabType;
    jsonData["NetAddress"]  = item.NetAddress;
    jsonData["IpAddress"]   = item.IpAddress;
    jsonData["LordId"]      = item.LordId;
    jsonData["NetStatus"]   = item.NetStatus;
    jsonData["EleStatus"]   = item.EleStatus;
    jsonData["TempStatus"]  = item.TempStatus;
    jsonData["AddTime"]     = item.AddTime;
    QtJson::JsonArray array;

    for(int i=0; i < item.SubCabs.length(); i++){
        array << QtJson::parse(serialize(item.SubCabs[i]));
    }
    jsonData["SubCabs"] = array;

    return QtJson::serialize(jsonData);
}

void DataModel::initModel(CabinetList &list)
{
    list.clear();
}

static bool sortCabinet(const DataModel::Cabinet &first, const DataModel::Cabinet &second)
{
    return (first.No < second.No);
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, CabinetList &list)
{
    if(array.isEmpty()) return false;
    Cabinet item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    qSort(list.begin(), list.end(), sortCabinet);
    return true;
}

QByteArray DataModel::serialize(CabinetList list)
{
    QtJson::JsonObject jsonData;
    QtJson::JsonArray array;

    for(int i = 0; i < list.length(); i++){
        array << QtJson::parse(serialize(list[i]));
    }
    jsonData["Success"] = true;
    jsonData["Body"] = array;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 监控室模型:
 ********************************************************************
*/

void DataModel::initModel(Room &item)
{
    item.Id.clear();
    item.UnitId.clear();
    item.UnitName.clear();
    item.No.clear();
    item.Name.clear();
    item.Address.clear();
    item.IpAddress.clear();
    item.ManageId1.clear();
    item.ManageId2.clear();
    item.ManageId3.clear();
    item.TaskId.clear();
    item.LeadId.clear();
    item.LockTime  = 0;
    item.SystemPassword.clear();
    item.AddTime   = 0;
}

bool DataModel::doDeSerialize(QString json, Room &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, Room &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id         = jsonData["Id"].toString();
    item.UnitId     = jsonData["UnitId"].toString();
    item.UnitName   = jsonData["UnitName"].toString();
    item.No         = jsonData["No"].toString();
    item.Name       = jsonData["Name"].toString();
    item.Address    = jsonData["Address"].toString();
    item.IpAddress  = jsonData["IpAddress"].toString();
    item.ManageId1  = jsonData["ManageId1"].toString();
    item.ManageId2  = jsonData["ManageId2"].toString();
    item.ManageId3  = jsonData["ManageId3"].toString();
    item.TaskId     = jsonData["TaskId"].toString();
    item.LeadId     = jsonData["LeadId"].toString();
    item.LockTime   = jsonData["UnitId"].toInt();
    item.SystemPassword = jsonData["SystemPassword"].toString();
    item.AddTime    = jsonData["AddTime"].toULongLong();
    return true;
}

QByteArray DataModel::serialize(Room &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]          = item.Id;
    jsonData["UnitId"]      = item.UnitId;
    jsonData["UnitName"]    = item.UnitName;
    jsonData["No"]          = item.No;
    jsonData["Name"]        = item.Name;
    jsonData["Address"]     = item.Address;
    jsonData["IpAddress"]   = item.IpAddress;
    jsonData["ManageId1"]   = item.ManageId1;
    jsonData["ManageId2"]   = item.ManageId2;
    jsonData["ManageId3"]   = item.ManageId3;
    jsonData["TaskId"]      = item.TaskId;
    jsonData["LeadId"]      = item.LeadId;
    jsonData["LockTime"]    = item.LockTime;
    jsonData["SystemPassword"] = item.SystemPassword;
    jsonData["AddTime"]     = item.AddTime;
    return QtJson::serialize(jsonData);
}

void DataModel::initModel(RoomList &list)
{
    list.clear();
}

static bool sortRoom(const DataModel::Room &first, const DataModel::Room &second)
{
    return (first.No < second.No);
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, RoomList &list)
{
    if(array.isEmpty()) return false;
    Room item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    qSort(list.begin(), list.end(), sortRoom);
    return true;
}

QByteArray DataModel::serialize(RoomList list)
{
    QtJson::JsonObject jsonData;
    QtJson::JsonArray array;

    for(int i = 0; i < list.length(); i++){
        array << QtJson::parse(serialize(list[i]));
    }
    jsonData["Success"] = true;
    jsonData["Body"] = array;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 任务枪支操作模型:
 ********************************************************************
*/

void DataModel::initModel(TaskGunOper &item)
{
    item.Id.clear();
    item.ManageId.clear();
    item.CabId.clear();
    item.SubCabId.clear();
    item.TaskItemId.clear();
    item.ObjectTypeId  = 0;
    item.GunId.clear();
    item.GunNo.clear();
    item.GunEno.clear();
    item.OperType      = 0;
    item.OperNumber    = 0;
    item.AddTime       = 0;
}

bool DataModel::doDeSerialize(QString json, TaskGunOper &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, TaskGunOper &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id            = jsonData["Id"].toString();
    item.ManageId      = jsonData["ManageId"].toString();
    item.CabId         = jsonData["CabId"].toString();
    item.SubCabId      = jsonData["SubCabId"].toString();
    item.TaskItemId    = jsonData["TaskItemId"].toString();
    item.ObjectTypeId  = jsonData["ObjectTypeId"].toInt();
    item.GunId         = jsonData["GunId"].toString();
    item.GunNo         = jsonData["GunNo"].toString();
    item.GunEno        = jsonData["GunEno"].toString();
    item.OperType      = jsonData["OperType"].toInt();
    item.OperNumber    = jsonData["OperNumber"].toInt();
    item.AddTime       = jsonData["AddTime"].toULongLong();
    return true;
}

QByteArray DataModel::serialize(TaskGunOper &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]          = item.Id;
    jsonData["ManageId"]    = item.ManageId;
    jsonData["CabId"]       = item.CabId;
    jsonData["SubCabId"]    = item.SubCabId;
    jsonData["TaskItemId"]  = item.TaskItemId;
    jsonData["ObjectTypeId"]= item.ObjectTypeId;
    jsonData["GunId"]       = item.GunId;
    jsonData["GunNo"]       = item.GunNo;
    jsonData["GunEno"]      = item.GunEno;
    jsonData["OperType"]    = item.OperType;
    jsonData["OperNumber"]  = item.OperNumber;
    jsonData["AddTime"]     = item.AddTime;
    return QtJson::serialize(jsonData);
}

void DataModel::initModel(TaskGunOperList &list)
{
    list.clear();
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, TaskGunOperList &list)
{
    if(array.isEmpty()) return false;
    TaskGunOper item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    return true;
}

QByteArray DataModel::serialize(TaskGunOperList list)
{
    QtJson::JsonObject jsonData;
    QtJson::JsonArray array;

    for(int i = 0; i < list.length(); i++){
        array << QtJson::parse(serialize(list[i]));
    }
    jsonData["Success"] = true;
    jsonData["Body"] = array;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 任务弹药操作模型:
 ********************************************************************
*/

void DataModel::initModel(TaskAmmoOper &item)
{
    item.Id.clear();
    item.ManageId.clear();
    item.CabId.clear();
    item.SubCabId.clear();
    item.TaskItemId.clear();
    item.ObjectTypeId  = 0;
    item.AmmoId.clear();
    item.OperType      = 0;
    item.OperNumber    = 0;
    item.AddTime       = 0;
}

bool DataModel::doDeSerialize(QString json, TaskAmmoOper &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, TaskAmmoOper &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id            = jsonData["Id"].toString();
    item.ManageId      = jsonData["ManageId"].toString();
    item.CabId         = jsonData["CabId"].toString();
    item.SubCabId      = jsonData["SubCabId"].toString();
    item.TaskItemId    = jsonData["TaskItemId"].toString();
    item.ObjectTypeId  = jsonData["ObjectTypeId"].toInt();
    item.AmmoId        = jsonData["AmmoId"].toString();
    item.OperType      = jsonData["OperType"].toInt();
    item.OperNumber    = jsonData["OperNumber"].toInt();
    item.AddTime       = jsonData["AddTime"].toULongLong();
    return true;
}

QByteArray DataModel::serialize(TaskAmmoOper &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]          = item.Id;
    jsonData["ManageId"]    = item.ManageId;
    jsonData["CabId"]       = item.CabId;
    jsonData["SubCabId"]    = item.SubCabId;
    jsonData["TaskItemId"]  = item.TaskItemId;
    jsonData["ObjectTypeId"]= item.ObjectTypeId;
    jsonData["AmmoId"]      = item.AmmoId;
    jsonData["OperType"]    = item.OperType;
    jsonData["OperNumber"]  = item.OperNumber;
    jsonData["AddTime"]     = item.AddTime;
    return QtJson::serialize(jsonData);
}

void DataModel::initModel(TaskAmmoOperList &list)
{
    list.clear();
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, TaskAmmoOperList &list)
{
    if(array.isEmpty()) return false;
    TaskAmmoOper item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    return true;
}

QByteArray DataModel::serialize(TaskAmmoOperList list)
{
    QtJson::JsonObject jsonData;
    QtJson::JsonArray array;

    for(int i = 0; i < list.length(); i++){
        array << QtJson::parse(serialize(list[i]));
    }
    jsonData["Success"] = true;
    jsonData["Body"] = array;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 任务清单模型:
 ********************************************************************
*/

void DataModel::initModel(TaskItem &item)
{
    item.Id.clear();
    item.TaskPoliceId.clear();
    item.TaskItemType  = 0;
    item.TaskItemStatus= Unexecuted;
    item.ObjectTypeId  = 0;
    item.ObjectNumber  = 0;
    item.BelongId.clear();
    item.Title.clear();
    item.OneOperNumber = 0;
    item.TwoOperNumber = 0;
    item.OutGunOpers.clear();
    item.OutAmmoOpers.clear();
    item.InGunOpers.clear();
    item.InAmmoOpers.clear();
}

bool DataModel::doDeSerialize(QString json, TaskItem &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, TaskItem &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id             = jsonData["Id"].toString();
    item.TaskPoliceId   = jsonData["TaskPoliceId"].toString();
    item.TaskItemType   = jsonData["TaskItemType"].toInt();
    item.TaskItemStatus = TaskItemStatus(jsonData["TaskItemStatus"].toInt());
    item.ObjectTypeId   = jsonData["ObjectTypeId"].toInt();
    item.ObjectNumber   = jsonData["ObjectNumber"].toInt();
    item.BelongId       = jsonData["BelongId"].toString();
    item.Title          = jsonData["Title"].toString();
    item.OneOperNumber  = jsonData["OneOperNumber"].toInt();
    item.TwoOperNumber  = jsonData["TwoOperNumber"].toInt();
    doDeSerialize(jsonData["OutGunOpers"].toList(), item.OutGunOpers);
    doDeSerialize(jsonData["OutAmmoOpers"].toList(), item.OutAmmoOpers);
    doDeSerialize(jsonData["InGunOpers"].toList(), item.InGunOpers);
    doDeSerialize(jsonData["InAmmoOpers"].toList(), item.InAmmoOpers);
    return true;
}

QByteArray DataModel::serialize(TaskItem &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]              = item.Id;
    jsonData["TaskPoliceId"]    = item.TaskPoliceId;
    jsonData["TaskItemType"]    = item.TaskItemType;
    jsonData["TaskItemStatus"]  = item.TaskItemStatus;
    jsonData["ObjectTypeId"]    = item.ObjectTypeId;
    jsonData["ObjectNumber"]    = item.ObjectNumber;
    jsonData["BelongId"]        = item.BelongId;
    jsonData["Title"]           = item.Title;
    jsonData["OneOperNumber"]   = item.OneOperNumber;
    jsonData["TwoOperNumber"]   = item.TwoOperNumber;
    QtJson::JsonArray array;

    for(int i=0; i < item.OutGunOpers.length(); i++)
        array << QtJson::parse(serialize(item.OutGunOpers[i]));
    jsonData["OutGunOpers"] = array;

    for(int i=0; i < item.OutAmmoOpers.length(); i++)
        array << QtJson::parse(serialize(item.OutAmmoOpers[i]));
    jsonData["OutAmmoOpers"] = array;

    for(int i=0; i < item.InGunOpers.length(); i++)
        array << QtJson::parse(serialize(item.InGunOpers[i]));
    jsonData["InGunOpers"] = array;

    for(int i=0; i < item.InAmmoOpers.length(); i++)
        array << QtJson::parse(serialize(item.InAmmoOpers[i]));
    jsonData["InAmmoOpers"] = array;

    return QtJson::serialize(jsonData);
}

void DataModel::initModel(TaskItemList &list)
{
    list.clear();
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, TaskItemList &list)
{
    if(array.isEmpty()) return false;
    TaskItem item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    return true;
}

QByteArray DataModel::serialize(TaskItemList list)
{
    QtJson::JsonObject jsonData;
    QtJson::JsonArray array;

    for(int i = 0; i < list.length(); i++){
        array << QtJson::parse(serialize(list[i]));
    }
    jsonData["Success"] = true;
    jsonData["Body"] = array;
    return QtJson::serialize(jsonData);
}

//根据BelongId查找TaskItem
TaskItem DataModel::findTaskItem(const TaskItemList &items, const QString &BelongId)
{
    for( int j = 0; j < items.length(); j++ )
    {
        if( BelongId == items[j].BelongId ) return items[j];
    }
    return TaskItem();
}



/**
 ********************************************************************
 * 任务警员模型:
 ********************************************************************
*/

void DataModel::initModel(TaskPolice &item)
{
    item.Id.clear();
    item.PoliceId.clear();
    item.Name.clear();
    item.TaskId.clear();
    item.IsComplete = false;
    item.IsResponsible = false;
    item.TaskItems.clear();
    item.No.clear();
}

bool DataModel::doDeSerialize(QString json, TaskPolice &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, TaskPolice &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id             = jsonData["Id"].toString();
    item.PoliceId       = jsonData["PoliceId"].toString();
    item.Name           = jsonData["Name"].toString();
    item.TaskId         = jsonData["TaskId"].toString();
    item.IsComplete     = jsonData["IsComplete"].toBool();
    item.IsResponsible  = jsonData["IsResponsible"].toBool();
    doDeSerialize(jsonData["TaskItems"].toList(), item.TaskItems);
    item.No             = jsonData["No"].toString();
    return true;
}

QByteArray DataModel::serialize(TaskPolice &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]              = item.Id;
    jsonData["PoliceId"]        = item.PoliceId;
    jsonData["Name"]            = item.Name;
    jsonData["TaskId"]          = item.TaskId;
    jsonData["IsComplete"]      = item.IsComplete;
    jsonData["IsResponsible"]   = item.IsResponsible;
    jsonData["No"]              = item.No;
    QtJson::JsonArray array;

    for(int i=0; i < item.TaskItems.length(); i++)
        array << QtJson::parse(serialize(item.TaskItems[i]));
    jsonData["TaskItems"] = array;

    return QtJson::serialize(jsonData);
}

void DataModel::initModel(TaskPoliceList &list)
{
    list.clear();
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, TaskPoliceList &list)
{
    if(array.isEmpty()) return false;
    TaskPolice item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    return true;
}

QByteArray DataModel::serialize(TaskPoliceList list)
{
    QtJson::JsonObject jsonData;
    QtJson::JsonArray array;

    for(int i = 0; i < list.length(); i++){
        array << QtJson::parse(serialize(list[i]));
    }
    jsonData["Success"] = true;
    jsonData["Body"] = array;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 任务模型:
 ********************************************************************
*/

void DataModel::initModel(Task &item)
{
    item.Id.clear();
    item.RoomId.clear();
    item.Code          = 0;
    item.TaskType      = OtherTaskType;
    item.TaskSubType   = 0;
    item.TaskStatus    = 0;
    item.PoliceId.clear();
    item.StartTime     = 0;
    item.EndTime       = 0;
    item.ApproveLeadId.clear();
    item.AddTime       = 0;
    item.TaskPolices.clear();
}

bool DataModel::doDeSerialize(QString json, Task &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, Task &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id            = jsonData["Id"].toString();
    item.RoomId        = jsonData["RoomId"].toString();
    item.Code          = jsonData["Code"].toInt();
    item.TaskType      = TaskType(jsonData["TaskType"].toInt());
    item.TaskSubType   = jsonData["TaskSubType"].toInt();
    item.TaskStatus    = jsonData["TaskStatus"].toInt();
    item.PoliceId      = jsonData["PoliceId"].toString();
    item.StartTime     = jsonData["StartTime"].toULongLong();
    item.EndTime       = jsonData["EndTime"].toULongLong();
    item.ApproveLeadId = jsonData["ApproveLeadId"].toString();
    item.AddTime       = jsonData["AddTime"].toULongLong();
    doDeSerialize(jsonData["TaskPolices"].toList(), item.TaskPolices);
    return true;
}

QByteArray DataModel::serialize(Task &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]              = item.Id;
    jsonData["RoomId"]          = item.RoomId;
    jsonData["Code"]            = item.Code;
    jsonData["TaskType"]        = item.TaskType;
    jsonData["TaskSubType"]     = item.TaskSubType;
    jsonData["TaskStatus"]      = item.TaskStatus;
    jsonData["PoliceId"]        = item.PoliceId;
    jsonData["StartTime"]       = item.StartTime;
    jsonData["EndTime"]         = item.EndTime;
    jsonData["ApproveLeadId"]   = item.ApproveLeadId;
    jsonData["AddTime"]         = item.AddTime;
    QtJson::JsonArray array;

    for(int i=0; i < item.TaskPolices.length(); i++)
        array << QtJson::parse(serialize(item.TaskPolices[i]));
    jsonData["TaskPolices"] = array;

    return QtJson::serialize(jsonData);
}

void DataModel::initModel(TaskList &list)
{
    list.clear();
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, TaskList &list)
{
    if(array.isEmpty()) return false;
    Task item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    return true;
}

QByteArray DataModel::serialize(TaskList list)
{
    QtJson::JsonObject jsonData;
    QtJson::JsonArray array;

    for(int i = 0; i < list.length(); i++){
        array << QtJson::parse(serialize(list[i]));
    }
    jsonData["Success"] = true;
    jsonData["Body"] = array;
    return QtJson::serialize(jsonData);
}

//根据任务清单ID，查找清单位置
ItemPoint DataModel::findTaskItem(const Task &task, const QString &itemId)
{
    ItemPoint point;
    point.pIndex = -1;
    point.iIndex = -1;
    for( int i = 0; i < task.TaskPolices.length(); i++ )
    {
        const TaskItemList &items = task.TaskPolices[i].TaskItems;
        for( int j = 0; j < items.length(); j++ )
        {
            if( itemId == items[j].Id ){
                point.pIndex = i;
                point.iIndex = j;
                return point;
            }
        }
    }
    return point;
}



/**
 ********************************************************************
 * 警员生物信息模型:
 ********************************************************************
*/

void DataModel::initModel(PoliceBio &item)
{
    item.Id.clear();
    item.PoliceId.clear();
    item.BioType       = 0;
    item.BioCheckType  = 0;
    item.FingerprintId = 0;
    item.Key.clear();
    item.TemplateType.clear();
}

bool DataModel::doDeSerialize(QString json, PoliceBio &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, PoliceBio &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id             = jsonData["Id"].toString();
    item.PoliceId       = jsonData["PoliceId"].toString();
    item.BioType        = jsonData["BioType"].toInt();
    item.BioCheckType   = jsonData["BioCheckType"].toInt();
    item.FingerprintId  = jsonData["FingerprintId"].toInt();
    item.Key            = jsonData["Key"].toString();
    item.TemplateType   = jsonData["TemplateType"].toString();
    return true;
}

QByteArray DataModel::serialize(PoliceBio &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]              = item.Id;
    jsonData["PoliceId"]        = item.PoliceId;
    jsonData["BioType"]         = item.BioType;
    jsonData["BioCheckType"]    = item.BioCheckType;
    jsonData["FingerprintId"]   = item.FingerprintId;
    jsonData["Key"]             = item.Key;
    jsonData["TemplateType"]    = item.TemplateType;
    return QtJson::serialize(jsonData);
}

void DataModel::initModel(PoliceBioList &list)
{
    list.clear();
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, PoliceBioList &list)
{
    if(array.isEmpty()) return false;
    PoliceBio item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    return true;
}

QByteArray DataModel::serialize(PoliceBioList list)
{
    QtJson::JsonObject jsonData;
    QtJson::JsonArray array;

    for(int i = 0; i < list.length(); i++){
        array << QtJson::parse(serialize(list[i]));
    }
    jsonData["Success"] = true;
    jsonData["Body"] = array;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 警员模型:
 ********************************************************************
*/

void DataModel::initModel(Police &item)
{
    item.Id.clear();
    item.Salt.clear();
    item.Name.clear();
    item.Photo.clear();
    item.PhotoBase64.clear();
    item.UnitId.clear();
    item.UnitName.clear();
    item.No.clear();
    item.Sex.clear();
    item.Rank          = 0;
    item.Phone.clear();
    item.Nation.clear();
    item.Address.clear();
    item.Departy       = 0;
    item.PoliceType    = 0;
    item.TaskId.clear();
    item.Licence.clear();
    item.IDCardNo.clear();
    item.Born.clear();
    item.GrantDept.clear();
    item.UserLifeBegin.clear();
    item.UserLifeEnd.clear();
    item.reserved.clear();
    item.PhotoFileName.clear();
    item.PhotoFileNameImage.clear();
    item.IsExistIdCard = false;
    item.AddTime       = 0;
    item.PoliceBios.clear();
}

bool DataModel::doDeSerialize(QString json, Police &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, Police &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id            = jsonData["Id"].toString();
    item.Salt          = jsonData["Salt"].toString();
    item.Name          = jsonData["Name"].toString();
    item.Photo         = jsonData["Photo"].toString();
    item.PhotoBase64   = jsonData["PhotoBase64"].toString();
    item.UnitId        = jsonData["UnitId"].toString();
    item.UnitName      = jsonData["UnitName"].toString();
    item.No            = jsonData["No"].toString();
    item.Sex           = jsonData["Sex"].toString();
    item.Rank          = jsonData["Rank"].toInt();
    item.Phone         = jsonData["Phone"].toString();
    item.Nation        = jsonData["Nation"].toString();
    item.Address       = jsonData["Address"].toString();
    item.Departy       = jsonData["Departy"].toInt();
    item.PoliceType    = jsonData["PoliceType"].toInt();
    item.TaskId        = jsonData["TaskId"].toString();
    item.Licence       = jsonData["Licence"].toString();
    item.IDCardNo      = jsonData["IDCardNo"].toString();
    item.Born          = jsonData["Born"].toString();
    item.GrantDept     = jsonData["GrantDept"].toString();
    item.UserLifeBegin = jsonData["UserLifeBegin"].toString();
    item.UserLifeEnd   = jsonData["UserLifeEnd"].toString();
    item.reserved      = jsonData["reserved"].toString();
    item.PhotoFileName = jsonData["PhotoFileName"].toString();
    item.PhotoFileNameImage = jsonData["PhotoFileNameImage"].toString();
    item.IsExistIdCard = jsonData["IsExistIdCard"].toBool();
    item.AddTime       = jsonData["AddTime"].toULongLong();
    doDeSerialize(jsonData["PoliceBios"].toList(), item.PoliceBios);
    return true;
}

QByteArray DataModel::serialize(Police &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]          = item.Id;
    jsonData["Salt"]        = item.Salt;
    jsonData["Name"]        = item.Name;
    jsonData["Photo"]       = item.Photo;
    jsonData["PhotoBase64"] = item.PhotoBase64;
    jsonData["UnitId"]      = item.UnitId;
    jsonData["UnitName"]    = item.UnitName;
    jsonData["No"]          = item.No;
    jsonData["Sex"]         = item.Sex;
    jsonData["Rank"]        = item.Rank;
    jsonData["Phone"]       = item.Phone;
    jsonData["Nation"]      = item.Nation;
    jsonData["Address"]     = item.Address;
    jsonData["Departy"]     = item.Departy;
    jsonData["PoliceType"]  = item.PoliceType;
    jsonData["TaskId"]      = item.TaskId;
    jsonData["Licence"]     = item.Licence;
    jsonData["IDCardNo"]    = item.IDCardNo;
    jsonData["Born"]        = item.Born;
    jsonData["GrantDept"]   = item.GrantDept;
    jsonData["UserLifeBegin"] = item.UserLifeBegin;
    jsonData["UserLifeEnd"] = item.UserLifeEnd;
    jsonData["reserved"]    = item.reserved;
    jsonData["PhotoFileName"] = item.PhotoFileName;
    jsonData["PhotoFileNameImage"] = item.PhotoFileNameImage;
    jsonData["IsExistIdCard"] = item.IsExistIdCard;
    jsonData["AddTime"]     = item.AddTime;
    QtJson::JsonArray array;

    for(int i=0; i < item.PoliceBios.length(); i++)
        array << QtJson::parse(serialize(item.PoliceBios[i]));
    jsonData["PoliceBios"] = array;
    return QtJson::serialize(jsonData);
}

void DataModel::initModel(PoliceList &list)
{
    list.clear();
}

static bool sortPolice(const Police &first, const Police &second)
{
    return (first.No < second.No);
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, PoliceList &list)
{
    if(array.isEmpty()) return false;
    Police item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    qSort(list.begin(), list.end(), sortPolice);
    return true;
}

QByteArray DataModel::serialize(PoliceList list)
{
    QtJson::JsonObject jsonData;
    QtJson::JsonArray array;

    for(int i = 0; i < list.length(); i++){
        array << QtJson::parse(serialize(list[i]));
    }
    jsonData["Success"] = true;
    jsonData["Body"] = array;
    return QtJson::serialize(jsonData);
}

//判断是否是当前值班管理员
bool DataModel::isCManager(const PoliceList &list, const QString &policeId)
{
    for( int i = 0; i < list.length(); i++ )
    {
        if( policeId == list[i].Id ) return true;
    }
    return false;
}



/**
 ********************************************************************
 * 系统日志:
 ********************************************************************
*/

void DataModel::initModel(SystemLog &item)
{
    item.Id.clear();
    item.RoomId.clear();
    item.RoomName.clear();
    item.CabId.clear();
    item.CabNo.clear();
    item.LogType   = 0;
    item.LogSubType= 0;
    item.ManageId.clear();
    item.LeadId.clear();
    item.LogTime   = 0;
    item.LogStatus = 0;
    item.LogContent.clear();
    item.AddTime   = 0;
}

bool DataModel::doDeSerialize(QString json, SystemLog &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, SystemLog &item)
{
    if(jsonData.isEmpty()) return false;
    item.Id        = jsonData["Id"].toString();
    item.RoomId    = jsonData["RoomId"].toString();
    item.RoomName  = jsonData["RoomName"].toString();
    item.CabId     = jsonData["CabId"].toString();
    item.CabNo     = jsonData["CabNo"].toString();
    item.LogType   = jsonData["LogType"].toInt();
    item.LogSubType= jsonData["LogSubType"].toInt();
    item.ManageId  = jsonData["ManageId"].toString();
    item.LeadId    = jsonData["LeadId"].toString();
    item.LogTime   = jsonData["LogTime"].toULongLong();
    item.LogStatus = jsonData["LogStatus"].toInt();
    item.LogContent= jsonData["LogContent"].toString();
    item.AddTime   = jsonData["AddTime"].toULongLong();
    return true;
}

QByteArray DataModel::serialize(SystemLog &item)
{
    QtJson::JsonObject jsonData;
    jsonData["Id"]          = item.Id;
    jsonData["RoomId"]      = item.RoomId;
    jsonData["RoomName"]    = item.RoomName;
    jsonData["CabId"]       = item.CabId;
    jsonData["CabNo"]       = item.CabNo;
    jsonData["LogType"]     = item.LogType;
    jsonData["LogSubType"]  = item.LogSubType;
    jsonData["ManageId"]    = item.ManageId;
    jsonData["LeadId"]      = item.LeadId;
    jsonData["LogTime"]     = item.LogTime;
    jsonData["LogStatus"]   = item.LogStatus;
    jsonData["LogContent"]  = item.LogContent;
    jsonData["AddTime"]     = item.AddTime;
    return QtJson::serialize(jsonData);
}

void DataModel::initModel(SystemLogList &list)
{
    list.clear();
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, SystemLogList &list)
{
    if(array.isEmpty()) return false;
    SystemLog item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    return true;
}

QByteArray DataModel::serialize(SystemLogList list)
{
    QtJson::JsonObject jsonData;
    QtJson::JsonArray array;

    for(int i = 0; i < list.length(); i++){
        array << QtJson::parse(serialize(list[i]));
    }
    jsonData["Success"] = true;
    jsonData["Body"] = array;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 枪柜心跳包:
 ********************************************************************
*/

void DataModel::initModel(CabHeart &item)
{
    item.CabId.clear();
    item.NetStatus = 0;
    item.EleStatus = 0;
    item.TempStatus= 0;
}

bool DataModel::doDeSerialize(QString json, CabHeart &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, CabHeart &item)
{
    if(jsonData.isEmpty()) return false;
    item.CabId     = jsonData["CabId"].toString();
    item.NetStatus = jsonData["NetStatus"].toInt();
    item.EleStatus = jsonData["EleStatus"].toInt();
    item.TempStatus= jsonData["TempStatus"].toInt();
    return true;
}

QByteArray DataModel::serialize(CabHeart &item)
{
    QtJson::JsonObject jsonData;
    jsonData["CabId"]       = item.CabId;
    jsonData["NetStatus"]   = item.NetStatus;
    jsonData["EleStatus"]   = item.EleStatus;
    jsonData["TempStatus"]  = item.TempStatus;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 专门的录入身份证信息的模型:
 ********************************************************************
*/

void DataModel::initModel(AddIdCard &item)
{
    item.PoliceId.clear();
    item.Name.clear();
    item.Sex.clear();
    item.Nation.clear();
    item.Address.clear();
    item.IDCardNo.clear();
    item.Born.clear();
    item.GrantDept.clear();
    item.UserLifeBegin.clear();
    item.UserLifeEnd.clear();
    item.reserved.clear();
    item.PhotoFileName.clear();
}

bool DataModel::doDeSerialize(QString json, AddIdCard &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, AddIdCard &item)
{
    if(jsonData.isEmpty()) return false;
    item.PoliceId       = jsonData["PoliceId"].toString();
    item.Name           = jsonData["Name"].toString();
    item.Sex            = jsonData["Sex"].toString();
    item.Nation         = jsonData["Nation"].toString();
    item.Address        = jsonData["Address"].toString();
    item.IDCardNo       = jsonData["IDCardNo"].toString();
    item.Born           = jsonData["Born"].toString();
    item.GrantDept      = jsonData["GrantDept"].toString();
    item.UserLifeBegin  = jsonData["UserLifeBegin"].toString();
    item.UserLifeEnd    = jsonData["UserLifeEnd"].toString();
    item.reserved       = jsonData["reserved"].toString();
    item.PhotoFileName  = jsonData["PhotoFileName"].toString();
    return true;
}

QByteArray DataModel::serialize(AddIdCard &item)
{
    QtJson::JsonObject jsonData;
    jsonData["PoliceId"]        = item.PoliceId;
    jsonData["Name"]            = item.Name;
    jsonData["Sex"]             = item.Sex;
    jsonData["Nation"]          = item.Nation;
    jsonData["Address"]         = item.Address;
    jsonData["IDCardNo"]        = item.IDCardNo;
    jsonData["Born"]            = item.Born;
    jsonData["GrantDept"]       = item.GrantDept;
    jsonData["UserLifeBegin"]   = item.UserLifeBegin;
    jsonData["UserLifeEnd"]     = item.UserLifeEnd;
    jsonData["reserved"]        = item.reserved;
    jsonData["PhotoFileName"]   = item.PhotoFileName;
    return QtJson::serialize(jsonData);
}



/**
 ********************************************************************
 * 当前监控室执行清单模型:
 ********************************************************************
*/

void DataModel::initModel(CurrentTaskItem &item)
{
    item.RoomId.clear();
    item.TaskId.clear();
    item.TaskItemId.clear();
}

bool DataModel::doDeSerialize(QString json, CurrentTaskItem &item)
{
    return doDeSerialize(QtJson::parse(json).toMap(), item);
}

bool DataModel::doDeSerialize(QtJson::JsonObject jsonData, CurrentTaskItem &item)
{
    if(jsonData.isEmpty()) return false;
    item.RoomId     = jsonData["RoomId"].toString();
    item.TaskId     = jsonData["TaskId"].toString();
    item.TaskItemId = jsonData["TaskItemId"].toString();
    return true;
}

QByteArray DataModel::serialize(CurrentTaskItem &item)
{
    QtJson::JsonObject jsonData;
    jsonData["RoomId"]      = item.RoomId;
    jsonData["TaskId"]      = item.TaskId;
    jsonData["TaskItemId"]  = item.TaskItemId;
    return QtJson::serialize(jsonData);
}

void DataModel::initModel(CurrentTaskItemList &list)
{
    list.clear();
}

bool DataModel::doDeSerialize(QtJson::JsonArray array, CurrentTaskItemList &list)
{
    if(array.isEmpty()) return false;
    CurrentTaskItem item;

    list.clear();
    for(int i = 0; i < array.length(); i++){
        initModel(item);
        if (doDeSerialize(array[i].toMap(), item)) list << item;
    }
    return true;
}

QByteArray DataModel::serialize(CurrentTaskItemList list)
{
    QtJson::JsonObject jsonData;
    QtJson::JsonArray array;

    for(int i = 0; i < list.length(); i++){
        array << QtJson::parse(serialize(list[i]));
    }
    jsonData["Success"] = true;
    jsonData["Body"] = array;
    return QtJson::serialize(jsonData);
}
