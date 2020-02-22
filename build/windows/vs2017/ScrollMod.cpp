#include "ScrollMod.h"

using namespace payload;

void ScrollMod::OnCreate()
{

}

void ScrollMod::OnDelete()
{

}

void ScrollMod::Update(const orxCLOCK_INFO &_rstInfo)
{

}

const float __fastcall ScrollMod::GetScreenWidth()
{
    float screenWidth;
    float screenHeight;
    orxDisplay_GetScreenSize(&screenWidth, &screenHeight);

    return screenWidth;
}

const float __fastcall ScrollMod::GetScreenHeight()
{
    float screenWidth;
    float screenHeight;
    orxDisplay_GetScreenSize(&screenWidth, &screenHeight);

    return screenHeight;
}

//TODO: Update this such that any camera (not just MainCamera) may be used.
const orxVECTOR __fastcall ScrollMod::WorldToScreenSpace(orxVECTOR _worldSpaceVec)
{
    float screenWidth;
    float screenHeight;
    float frustumWidth = GetFloat("FrustumWidth", "MainCamera");
    float frustumHeight = GetFloat("FrustumHeight", "MainCamera");
    orxDisplay_GetScreenSize(&screenWidth, &screenHeight);
    float widthRatio = screenWidth / frustumWidth;
    float heightRatio = screenHeight / frustumHeight;

    return { (_worldSpaceVec.fX * widthRatio) + screenWidth / 2, (_worldSpaceVec.fY * heightRatio) + screenHeight / 2 };
}

const std::vector<std::string> __fastcall ScrollMod::GetObjectSections()
{
    std::vector<std::string> retVal;
    for (int i = 0; i < orxConfig_GetSectionCount(); i++)
    {
        const char *sectionName = orxConfig_GetSection(i);

        if (orxString_SearchString(sectionName, "O-") != nullptr)
        {
            retVal.push_back(sectionName);
        }
    }
    return retVal;
}

const std::vector<std::string> __fastcall ScrollMod::GetDescendentSections(const std::string _parentSectionName)
{
    std::vector<std::string> retVal;
    for (int i = 0; i < orxConfig_GetSectionCount(); i++)
    {
        const char *sectionName = orxConfig_GetSection(i);
        const char *parentSectionName = orxConfig_GetParent(sectionName);

        if (parentSectionName != nullptr && orxString_Compare(orxConfig_GetParent(sectionName), _parentSectionName.c_str()) == 0)
        {
            retVal.push_back(sectionName);
            std::vector<std::string> descendentDescendents = GetDescendentSections(sectionName);

            for (std::string str : descendentDescendents)
            {
                retVal.push_back(str);
            }
        }
    }
    return retVal;
}

const bool __fastcall ScrollMod::SectionUsesParentSpace(const std::string _sectionName)
{
    bool usesParentSpaceBool = ScrollMod::GetBool("UseParentSpace", _sectionName);
    std::string usesParentSpaceString = ScrollMod::GetString("UseParentSpace", _sectionName);
    return usesParentSpaceBool || orxString_Compare(usesParentSpaceString.c_str(), "position") == 0 || orxString_Compare(usesParentSpaceString.c_str(), "both") == 0;
}

void ScrollMod::Substring(const orxCHAR *_strIn, orxCHAR *_strOut, const int &_index, const int &_size)
{
    int i = 0;

    while (i < _size)
    {
        _strOut[i] = _strIn[_index + i];
        i++;
    }

    _strOut[i] = '\0';
}

const float ScrollMod::AngleBetweenVectors(orxVECTOR _from, orxVECTOR _to)
{
    float dotProduct = _from.fX * _to.fX + _from.fY * _to.fY;
    float determinant = _from.fX * _to.fY - _from.fY * _to.fX;
    return atan2f(determinant, dotProduct);
}

const float ScrollMod::AngleBetweenPoints(orxVECTOR _from, orxVECTOR _to)
{
    return atan2f(_to.fY - _from.fY, _to.fX - _from.fX);
}

const orxVECTOR ScrollMod::NormalizeVector(const orxVECTOR &_vec)
{
    orxVECTOR normalizedVector = orxVECTOR_0;

    return *orxVector_Normalize(&normalizedVector, &_vec);
}

const float ScrollMod::VectorToRadians(const orxVECTOR &_vec)
{
    return orxMath_ATan(_vec.fY, _vec.fX);
}

const orxVECTOR ScrollMod::RadiansToVector(const float &_angle)
{
    return { orxMath_Cos(_angle), orxMath_Sin(_angle) };
}

const orxVECTOR ScrollMod::ReflectionVector(const orxVECTOR &_hit, const orxVECTOR &_normal)
{
    float dot = orxVector_2DDot(&_hit, &_normal);
    orxVECTOR subtrahend = { 2 * dot * _normal.fX, 2 * dot * _normal.fY };

    return { _hit.fX - subtrahend.fX, _hit.fY - subtrahend.fY };
}

const RaycastData *ScrollMod::Raycast(
    const orxVECTOR &_begin,
    const float &_direction,
    const int &_checkMask,
    const int &_selfFlags,
    const float &_distance,
    const bool &_bEarlyExit)
{
    orxVECTOR endingPos = { _begin.fX + orxMath_Cos(_direction) * _distance, _begin.fY + orxMath_Sin(_direction) * _distance };
    orxVECTOR hitPos;
    orxVECTOR hitNormal;
    orxOBJECT *hitObject = orxObject_Raycast(&_begin, &endingPos, _selfFlags, _checkMask, _bEarlyExit, &hitPos, &hitNormal);

    if (hitObject != nullptr)
    {
        RaycastData retVal(hitObject, hitPos, hitNormal);
        return &retVal;
    }

    return nullptr;
}

ScrollMod *ScrollMod::CreateObject(
    std::string _modelName,
    std::map<std::string, const orxBOOL> _boolParamMap,
    std::map<std::string, const orxFLOAT> _floatParamMap,
    std::map<std::string, const orxVECTOR*> _vectorParamMap,
    std::map<std::string, std::string> _stringParamMap)
{
    std::map<std::string, const orxBOOL> initialBoolMap;
    std::map<std::string, const orxFLOAT> initialFloatMap;
    std::map<std::string, const orxVECTOR*> initialVectorMap;
    std::map<std::string, std::string> initialStringMap;

    for (auto it = _boolParamMap.begin(); it != _boolParamMap.end(); it++)
    {
        initialBoolMap.insert({ it->first, GetBool(it->first, _modelName) });
        SetBool(it->first, it->second, _modelName);
    }
    for (auto it = _floatParamMap.begin(); it != _floatParamMap.end(); it++)
    {
        initialFloatMap.insert({ it->first, GetFloat(it->first, _modelName) });
        SetFloat(it->first, it->second, _modelName);
    }
    for (auto it = _vectorParamMap.begin(); it != _vectorParamMap.end(); it++)
    {
        initialVectorMap.insert({ it->first, &GetVector(it->first, _modelName) });
        SetVector(it->first, it->second, _modelName);
    }
    for (auto it = _stringParamMap.begin(); it != _stringParamMap.end(); it++)
    {
        initialStringMap.insert({ it->first, GetString(it->first, _modelName) });
        SetString(it->first, it->second, _modelName);
    }

    ScrollMod *retVal = (ScrollMod*)Payload::GetInstance().CreateObject(_modelName.c_str());

    // Reset config values so as not to muck up future object creation.
    for (auto it = initialBoolMap.begin(); it != initialBoolMap.end(); it++)
    {
        SetBool(it->first, it->second, _modelName);
    }
    for (auto it = initialFloatMap.begin(); it != initialFloatMap.end(); it++)
    {
        SetFloat(it->first, it->second, _modelName);
    }
    for (auto it = initialVectorMap.begin(); it != initialVectorMap.end(); it++)
    {
        SetVector(it->first, it->second, _modelName);
    }
    for (auto it = initialStringMap.begin(); it != initialStringMap.end(); it++)
    {
        SetString(it->first, it->second, _modelName);
    }

    return retVal;
}

const std::string ScrollMod::GetModelName() const
{
    std::string retVal(ScrollObject::GetModelName());

    return retVal;
}

const orxVECTOR __fastcall ScrollMod::GetPosition(const bool &_bWorld) const
{
    orxVECTOR vecRef;
    ScrollObject::GetPosition(vecRef, _bWorld);

    return vecRef;
}

const orxVECTOR __fastcall ScrollMod::GetPositionNextFrame(const float &_fDT, const bool &_bWorld) const
{
    orxVECTOR curPos = GetPosition(_bWorld);
    orxVECTOR curSpeed = GetSpeed();

    return { curPos.fX + curSpeed.fX * _fDT, curPos.fY + curSpeed.fY * _fDT, curPos.fZ + curSpeed.fZ * _fDT };
}

const orxVECTOR __fastcall ScrollMod::GetScale(const bool &_bWorld) const
{
    orxVECTOR vecRef;
    ScrollObject::GetScale(vecRef, _bWorld);

    return vecRef;
}

const orxVECTOR __fastcall ScrollMod::GetSize() const
{
    orxVECTOR vecRef;
    ScrollObject::GetSize(vecRef);

    return vecRef;
}

const orxVECTOR __fastcall ScrollMod::GetScaledSize(const bool &_bWorld) const
{
    orxVECTOR size = GetSize();
    orxVECTOR scale = GetScale(_bWorld);

    return { size.fX * scale.fX, size.fY * scale.fY, size.fZ * scale.fZ };
}

const orxVECTOR __fastcall ScrollMod::GetSpeed(const bool &_bRelative) const
{
    orxVECTOR vecRef;
    ScrollObject::GetSpeed(vecRef, _bRelative);

    return vecRef;
}

orxBOOL __fastcall ScrollMod::GetBool(const std::string _key, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    orxBOOL retVal = orxConfig_GetBool(_key.c_str());
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

orxBOOL __fastcall ScrollMod::GetListBool(const std::string _key, const int _index, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    orxBOOL retVal = orxConfig_GetListBool(_key.c_str(), _index);
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

orxFLOAT __fastcall ScrollMod::GetFloat(const std::string _key, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    orxFLOAT retVal = orxConfig_GetFloat(_key.c_str());
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

orxU32 __fastcall ScrollMod::GetU32(const std::string _key, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    orxU32 retVal = orxConfig_GetU32(_key.c_str());
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

orxFLOAT __fastcall ScrollMod::GetListFloat(const std::string _key, const int _index, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    orxFLOAT retVal = orxConfig_GetListFloat(_key.c_str(), _index);
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

orxVECTOR __fastcall ScrollMod::GetVector(const std::string _key, const std::string _sectionName)
{
    orxVECTOR vecRef = orxVECTOR_0;
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    orxVECTOR *vec = orxConfig_GetVector(_key.c_str(), &vecRef);
    orxVECTOR retVal = vec == NULL ? orxVECTOR_0 : *vec;
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

orxVECTOR __fastcall ScrollMod::GetListVector(const std::string _key, const int _index, const std::string _sectionName)
{
    orxVECTOR vecRef = orxVECTOR_0;
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    orxVECTOR *vec = orxConfig_GetListVector(_key.c_str(), _index, &vecRef);
    orxVECTOR retVal = vec == NULL ? orxVECTOR_0 : *vec;
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

const std::string __fastcall ScrollMod::GetString(const std::string _key, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    const orxCHAR *retVal = orxConfig_GetString(_key.c_str());
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

const std::string __fastcall ScrollMod::GetListString(const std::string _key, const int _index, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    const orxCHAR *retVal = orxConfig_GetListString(_key.c_str(), _index);
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

const int __fastcall ScrollMod::GetListCount(const std::string _key, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    const int retVal = orxConfig_GetListCount(_key.c_str());
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

orxSTATUS __fastcall ScrollMod::SetBool(const std::string _key, orxBOOL _value, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    orxSTATUS retVal = orxConfig_SetBool(_key.c_str(), _value);
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

orxSTATUS __fastcall ScrollMod::SetFloat(const std::string _key, orxFLOAT _value, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    orxSTATUS retVal = orxConfig_SetFloat(_key.c_str(), _value);
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

orxSTATUS __fastcall ScrollMod::SetVector(const std::string _key, const orxVECTOR *_value, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    orxSTATUS retVal = orxConfig_SetVector(_key.c_str(), _value);
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

orxSTATUS __fastcall ScrollMod::SetString(const std::string _key, const std::string _value, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    orxSTATUS retVal = orxConfig_SetString(_key.c_str(), _value.c_str());
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

orxSTATUS __fastcall ScrollMod::SetListString(const std::string _key, const char **_list, int _numValues, const std::string _sectionName)
{
    orxBOOL sectionPassed = orxString_Compare(_sectionName.c_str(), "") != 0;

    if (sectionPassed)
    {
        orxConfig_PushSection(_sectionName.c_str());
    }
    orxSTATUS retVal = orxConfig_SetListString(_key.c_str(), _list, _numValues);
    if (sectionPassed)
    {
        orxConfig_PopSection();
    }

    return retVal;
}

void __fastcall ScrollMod::SetParentSpacePosition(const orxVECTOR &_position)
{
    orxSTRUCTURE *parent = orxObject_GetParent(GetOrxObject());
    orxOBJECT *parentObj = orxOBJECT(parent);
    if (parentObj != nullptr)
    {
        orxVECTOR parentPos = orxVECTOR_0;
        orxObject_GetPosition(parentObj, &parentPos);
        orxVECTOR parentSize = orxVECTOR_0;
        orxObject_GetSize(parentObj, &parentSize);
        orxVECTOR parentScale = orxVECTOR_0;
        orxObject_GetScale(parentObj, &parentScale);
        orxVECTOR parentScaledSize = { parentSize.fX * parentScale.fX, parentSize.fY * parentScale.fY, parentSize.fZ * parentScale.fZ };
        orxVECTOR newPos = { parentPos.fX + _position.fX * parentScaledSize.fX, parentPos.fY + _position.fY * parentScaledSize.fY, parentPos.fZ +  _position.fZ * parentScaledSize.fZ };
        SetPosition(newPos);
    }
    else
    {
        // TODO: Account for instances in which parent isn't an orxOBJECT (for instance, maybe it's an orxCAMERA).
    }
}

float __fastcall ScrollMod::GetActiveTime()
{
    return orxObject_GetActiveTime(GetOrxObject());
}

orxSTRUCTURE *__fastcall ScrollMod::GetOwner()
{
    return orxObject_GetOwner(GetOrxObject());
}

orxSTRUCTURE *__fastcall ScrollMod::GetParent()
{
    return orxObject_GetParent(GetOrxObject());
}

void __fastcall ScrollMod::SetOwner(const ScrollObject *_owner)
{
    if (_owner != nullptr)
    {
        orxObject_SetOwner(GetOrxObject(), _owner->GetOrxObject());
    }
    else
    {
        orxObject_SetOwner(GetOrxObject(), nullptr);
    }
}

orxSTATUS __fastcall ScrollMod::SetParent(const ScrollObject *_parent)
{
    return orxObject_SetParent(GetOrxObject(), _parent->GetOrxObject());
}

orxU16 __fastcall ScrollMod::GetSelfFlags(const orxBODY_PART *_part)
{
    return orxBody_GetPartSelfFlags(_part);
}

orxU16 __fastcall ScrollMod::GetCheckMask(const orxBODY_PART *_part)
{
    return orxBody_GetPartCheckMask(_part);
}

orxSTATUS __fastcall ScrollMod::SetSelfFlags(orxBODY_PART *_part, orxU16 _selfFlags)
{
    return orxBody_SetPartSelfFlags(_part, _selfFlags);
}

orxSTATUS __fastcall ScrollMod::SetCheckMask(orxBODY_PART *_part, orxU16 _checkMask)
{
    return orxBody_SetPartCheckMask(_part, _checkMask);
}

const bool __fastcall ScrollMod::IsPendingDestruction()
{
    float lifeTime = GetLifeTime();

    // If lifeTime < 0, then the ScrollMod's lifeTime is "infinite".
    if (lifeTime >= 0)
    {
        float age = GetActiveTime();

        return age >= lifeTime;
    }

    return false;
}

orxAABOX __fastcall ScrollMod::GetAABB()
{
    orxAABOX aabb;
    orxOBOX obb;
    orxVECTOR obbCenter;
    orxVECTOR scaledSize = GetScaledSize();

    orxObject_GetBoundingBox(GetOrxObject(), &obb);
    orxOBox_GetCenter(&obb, &obbCenter);

    float rotation = GetRotation();
    float absSin = orxMath_Abs(orxMath_Sin(rotation));
    float absCos = orxMath_Abs(orxMath_Cos(rotation));
    float aabbWidthBy2 = (scaledSize.fY * absSin + scaledSize.fX * absCos) / 2.0f;
    float aabbHeightBy2 = (scaledSize.fX * absSin + scaledSize.fY * absCos) / 2.0f;
    orxVECTOR aabbTopLeft = {
        obbCenter.fX - aabbWidthBy2,
        obbCenter.fY - aabbHeightBy2 };
    orxVECTOR aabbBottomRight = {
        obbCenter.fX + aabbWidthBy2,
        obbCenter.fY + aabbHeightBy2 };

    orxAABox_Set(&aabb, &aabbTopLeft, &aabbBottomRight);

    return aabb;
}

orxSTRUCTURE *ScrollMod::GetStructure(orxSTRUCTURE_ID _structureID)
{
    orxOBJECT *obj = GetOrxObject();

    return _orxStructure_GetPointer(_orxObject_GetStructure(obj, _structureID), _structureID);
}

orxBODY *ScrollMod::GetBody()
{
    return (orxBODY*)GetStructure(orxSTRUCTURE_ID_BODY);
}

orxBODY_PART *__fastcall ScrollMod::AddBodyPartByName(const std::string _partName)
{
    orxBODY *body = GetBody();
    return orxBody_AddPartFromConfig(body, _partName.c_str());
}

orxSTATUS __fastcall ScrollMod::RemoveBodyPartByName(const std::string _partName)
{
    orxBODY *body = GetBody();
    return orxBody_RemovePartFromConfig(body, _partName.c_str());
}

orxSTATUS __fastcall ScrollMod::AddUniqueFX(const std::string _fxName, const float &_delay)
{
    return orxObject_AddUniqueDelayedFX(GetOrxObject(), _fxName.c_str(), _delay);
}

const bool __fastcall ScrollMod::IsOffCamera(std::string _cameraName)
{
    orxAABOX frustum;
    orxAABOX aabb = GetAABB();
    orxCAMERA *camera = orxCamera_Get(_cameraName.c_str());

    orxCamera_GetFrustum(camera, &frustum);

    return !orxAABox_Test2DIntersection(&frustum, &aabb);
}

int __fastcall ScrollMod::GetNumChildren()
{
    int result;

    for (ScrollObject *child = GetOwnedChild(); child; child = child->GetOwnedSibling())
    {
        result++;
    }

    return result;
}

ScrollObject *ScrollMod::GetChildByName(const std::string _childName)
{
    for (ScrollObject *child = GetOwnedChild(); child; child = child->GetOwnedSibling())
    {
        if (orxString_Compare(child->GetModelName(), _childName.c_str()) == 0)
        {
            return child;
        }
    }
}

ScrollObject *ScrollMod::GetChildByName(const std::vector<std::string> &_childNameList)
{
    for (ScrollObject *child = GetOwnedChild(); child; child = child->GetOwnedSibling())
    {
        for (std::string str : _childNameList)
        {
            if (orxString_Compare(child->GetModelName(), str.c_str()) == 0)
            {
                return child;
            }
        }
    }
}

orxVECTOR ScrollMod::GetWorldGravity()
{
    orxVECTOR worldGravity = orxVECTOR_0;

    return *orxPhysics_GetGravity(&worldGravity);
}

orxBODY_PART *ScrollMod::GetBodyPartByName(const std::string _partName)
{
    orxBODY *body = (orxBODY*)GetStructure(orxSTRUCTURE_ID_BODY);

    for (orxBODY_PART *part = orxBody_GetNextPart(body, orxNULL); part; part = orxBody_GetNextPart(body, part))
    {
        if (orxString_Compare(orxBody_GetPartName(part), _partName.c_str()) == 0)
        {
            return part;
        }
    }
}

void ScrollMod::SetTargetAnim(const std::string _animName)
{
    orxObject_SetTargetAnim(GetOrxObject(), _animName.c_str());
}

orxSTATUS __fastcall ScrollMod::SetCustomGravity(orxVECTOR &_customGravity)
{
    return orxObject_SetCustomGravity(GetOrxObject(), &_customGravity);
}

void ScrollMod::SetBodyPartSolid(const std::string _partName, const orxBOOL &_bSolid)
{
    orxBody_SetPartSolid(GetBodyPartByName(_partName), _bSolid);
}

void ScrollMod::MoveTo(const orxVECTOR &_destination, const float &_speed, const float &_decelerationDistance)
{
    orxVECTOR position = GetPosition();
    float angleBetweenSelfAndTarget = AngleBetweenPoints(position, _destination);
    orxVECTOR normalizedSpeed = RadiansToVector(angleBetweenSelfAndTarget);
    float distance = orxVector_GetDistance(&position, &_destination);
    float speed = distance <= _decelerationDistance ? _speed * (distance / _decelerationDistance) : _speed;
    SetSpeed({ normalizedSpeed.fX * speed, normalizedSpeed.fY * speed });
}

void ScrollMod::Destroy()
{
    SetLifeTime(0.0);
}
