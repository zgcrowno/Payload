#pragma once

#include "Payload.h"
#include "RaycastData.h"
#include <map>
#include <vector>

#define orxMATH_KF_PI_BY_8 (orxMATH_KF_PI_BY_4 / 2.0f)
#define orxMATH_KF_PI_BY_16 (orxMATH_KF_PI_BY_8 / 2.0f)
#define orxMATH_KF_PI_BY_32 (orxMATH_KF_PI_BY_16 / 2.0f)
#define orxMATH_KF_PI_BY_64 (orxMATH_KF_PI_BY_32 / 2.0f)

namespace payload
{
    /// <summary>A wrapper for ScrollObject which serves to make ScrollObjects less procedural, and more object-oriented.</summary>
    class ScrollMod : public ScrollObject
    {
    public:
        //! Called on object creation
        virtual void OnCreate();
        //! Called on object deletion
        virtual void OnDelete();
        //! Called on clock update
        virtual void Update(const orxCLOCK_INFO &_rstInfo);

        static const std::vector<std::string> __fastcall GetObjectSections();
        static const std::vector<std::string> __fastcall GetDescendentSections(const std::string _parentSectionName);
        static const bool __fastcall SectionUsesParentSpace(const std::string _sectionName);
        /// <summary>Places a substring of <paramref name="_strIn"/> beginning at index <paramref name="_index"/> and having size <paramref name="_size"/> into <paramref name="_strOut"/>.</summary>
        /// <param name="_strIn">The string from which we're copying a substring.</param>
        /// <param name="_strOut">The string into which we're placing the copy of a substring from <paramref name="_strIn"/>.</param>
        /// <param name="_index">The index at which to start copying the substring.</param>
        /// <param name="_size">The size of the copied substring.</param>
        static void Substring(const orxCHAR *_strIn, orxCHAR *_strOut, const int &_index, const int &_size);
        /// <summary>Returns a radian representation of the angle between vectors <paramref name="_from"/> and <paramref name="_to"/>.</summary>
        /// <param name="_from">The vector at which we're starting the angle calculation.</param>
        /// <param name="_to">The vector at which we're finishing the angle calculation.</param>
        /// <returns>A radian representation of the angle between <paramref name="_from"/> and <paramref name="_to"/>.</returns>
        static const float AngleBetweenVectors(orxVECTOR _from, orxVECTOR _to);
        /// <summary>Returns a radian representation of the angle between points <paramref name="_from"/> and <paramref name="_to"/>.</summary>
        /// <param name="_from">The point at which we're starting the angle calculation.</param>
        /// <param name="_to">The point at which we're finishing the angle calculation.</param>
        /// <returns>A radian representation of the angle between points <paramref name="_from"/> and <paramref name="_to"/>.</returns>
        static const float AngleBetweenPoints(orxVECTOR _from, orxVECTOR _to);
        /// <summary>Returns the normalized form of <paramref name="_vec"/>.</summary>
        /// <param name="_vec">The vector to normalize.</param>
        /// <returns>The normalized form of <paramref name="_vec"/>.</returns>
        static const orxVECTOR NormalizeVector(const orxVECTOR &_vec);
        /// <summary>Returns the radian interpretation of the passed vector.</summary>
        /// <param name="_vec">The vector we wish to interpret as radians.</param>
        /// <returns>The radian interpretation of the passed vector.</returns>
        static const float VectorToRadians(const orxVECTOR &_vec);
        /// <summary>Returns an inherently normalized vector representation of <paramref name="_angle"/>.</summary>
        /// <param name="_angle">The angle (in radians) from which we're deriving a normalized vector.</param>
        /// <returns>An inherently normalized vector representation of <paramref name="_angle"/>.</returns>
        static const orxVECTOR RadiansToVector(const float &_angle);
        /// <summary>Returns the reflection vector of <paramref name="_hit"/> about <paramref name="_normal"/>.</summary>
        /// <param name="_hit">The vector to be reflected about <paramref name="_normal"/>.</param>
        /// <param name="_normal">The vector about which <paramref name="_hit"/> is to be reflected.</param>
        /// <returns>The reflection vector of <paramref name="_hit"/> about <paramref name="_normal"/>.</returns>
        static const orxVECTOR ReflectionVector(const orxVECTOR &_hit, const orxVECTOR &_normal);
        /// <summary>A simplified version of ORX's built-in raycast function.</summary>
        /// <param name="_begin">The position from which to start the raycast.</param>
        /// <param name="_direction">The direction in which to "fire" the raycast.</param>
        /// <param name="_checkMask">The check mask used for filtering.</param>
        /// <param name="_selfFlags">The self flags used for filtering.</param>
        /// <param name="_distance">The distance the raycast should cover.</param>
        /// <param name="_bEarlyExit">Bool determining whether or not the raycast should cease once a hit has been achieved.</param>
        /// <returns>The raycast data.</returns>
        static const RaycastData *Raycast(
            const orxVECTOR &_begin,
            const float &_direction,
            const int &_checkMask,
            const int &_selfFlags = 0xFFFF,
            const float &_distance = 10000.0f,
            const bool &_bEarlyExit = true);
        /// <summary>A variation on Payload's CreateObject function which allows the passage of config-appropriate parameters.</summary>
        /// <param name="_modelName">The name of the model we wish to instantiate.</param>
        /// <param name="_boolParamMap">The bool parameters we wish to pass.</param>
        /// <param name="_floatParamMap">The float parameters we wish to pass.</param>
        /// <param name="_vectorParamMap">The vector parameters we wish to pass.</param>
        /// <param name="_stringParamMap">The string parameters we wish to pass.</param>
        /// <returns>The freshly constructed ScrollMod instance.</returns>
        static ScrollMod *CreateObject(
            const std::string _modelName,
            std::map<std::string, const orxBOOL> _boolParamMap = {},
            std::map<std::string, const orxFLOAT> _floatParamMap = {},
            std::map<std::string, const orxVECTOR*> _vectorParamMap = {},
            std::map<std::string, std::string> _stringParamMap = {});
        /// <summary>Just like ScrollObject's GetModelName() method, but returns a std::string instead of a char *.</summary>
        /// <returns>The std::string representation of the ScrollMod's model name.</returns>
        const std::string GetModelName() const;
        /// <summary>A more object-oriented version of ORX's GetPosition function.</summary>
        /// <param name="_bWorld">Bool representing whether or not the returned vector is in world space (as opposed to local space).</param>
        /// <returns>The ScrollMod's position.</returns>
        const orxVECTOR __fastcall GetPosition(const bool &_bWorld = false) const;
        /// <summary>Gets the predicted position of the ScrollMod one frame from when the function is called.</summary>
        /// <param name="_fDT">The delta time.</param>
        /// <param name="_bWorld">Bool representing whether or not the returned vector is in world space (as opposed to local space).</param>
        /// <returns>The predicted position of the ScrollMod one frame from when the function is called.</returns>
        const orxVECTOR __fastcall GetPositionNextFrame(const float &_fDT, const bool &_bWorld = false) const;
        /// <summary>A more object-oriented version of ORX's GetScale function.</summary>
        /// <param name="_bWorld">Bool representing whether or not the returned vector is in world space (as opposed to local space).</param>
        /// <returns>The ScrollMod's scale.</returns>
        const orxVECTOR __fastcall GetScale(const bool &_bWorld = false) const;
        /// <summary>A more object-oriented version of ORX's GetSize function.</summary>
        /// <returns>The ScrollMod's size.</returns>
        const orxVECTOR __fastcall GetSize() const;
        /// <summary>Returns the size of the ScrollMod after taking its scale into account.</summary>
        /// <param name="_bWorld">Bool representing whether or not the object's scale is considered in world space (as opposed to local space).</param>
        /// <returns>The size of the ScrollMod after taking its scale into account.</returns>
        const orxVECTOR __fastcall GetScaledSize(const bool &_bWorld = false) const;
        /// <summary>A more object-oriented version of ORX's GetSpeed function.</summary>
        /// <param name="_bRelative">Whether or not we're getting the ScrollMod's parent-relative speed.</param>
        /// <returns>The ScrollMod's speed.</returns>
        const orxVECTOR __fastcall GetSpeed(const bool &_bRelative = false) const;
        /// <summary>Gets a bool value named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</summary>
        /// <param name="_key">The name of the bool we wish to retrieve from config.</param>
        /// <param name="_sectionName">The section, if any, from which we wish to retrieve <paramref name="_key"/>.</param>
        /// <returns>The bool value named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</returns>
        static orxBOOL __fastcall GetBool(const std::string _key, const std::string _sectionName = "");
        /// <summary>Gets the bool value at index <paramref name="_index"/> in the list named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</summary>
        /// <param name="_key">The name of the list from which we wish to retrieve a bool in config.</param>
        /// <param name="_index">The index of the list named <paramref name="_key"/> at which we wish to retrieve a bool.</param>
        /// <param name="_sectionName">The section, if any, from which we wish to retrieve a bool in the list named <paramref name="_key"/>.</param>
        /// <returns>The bool value at index <paramref name="_index"/> in the list named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</returns>
        static orxBOOL __fastcall GetListBool(const std::string _key, const int _index, const std::string _sectionName = "");
        /// <summary>Gets a float value named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</summary>
        /// <param name="_key">The name of the float we wish to retrieve from config.</param>
        /// <param name="_sectionName">The section, if any, from which we wish to retrieve <paramref name="_key"/>.</param>
        /// <returns>The float value named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</returns>
        static orxFLOAT __fastcall GetFloat(const std::string _key, const std::string _sectionName = "");
        static orxU32 __fastcall GetU32(const std::string _key, const std::string _sectionName = "");
        /// <summary>Gets the float value at index <paramref name="_index"/> in the list named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</summary>
        /// <param name="_key">The name of the list from which we wish to retrieve a float in config.</param>
        /// <param name="_index">The index of the list named <paramref name="_key"/> at which we wish to retrieve a float.</param>
        /// <param name="_sectionName">The section, if any, from which we wish to retrieve a float in the list named <paramref name="_key"/>.</param>
        /// <returns>The float value at index <paramref name="_index"/> in the list named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</returns>
        static orxFLOAT __fastcall GetListFloat(const std::string _key, const int _index, const std::string _sectionName = "");
        /// <summary>Gets a vector value named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</summary>
        /// <param name="_key">The name of the vector we wish to retrieve from config.</param>
        /// <param name="_passedVector">The vector to hold the value we're retrieving.</param>
        /// <param name="_sectionName">The section, if any, from which we wish to retrieve <paramref name="_key"/>.</param>
        /// <returns>The vector value named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</returns>
        static orxVECTOR __fastcall GetVector(const std::string _key, const std::string _sectionName = "");
        /// <summary>Gets the vector at index <paramref name="_index"/> in the list named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</summary>
        /// <param name="_key">The name of the list from which we wish to retrieve a vector in config.</param>
        /// <param name="_index">The index of the list named <paramref name="_key"/> at which we wish to retrieve a vector.</param>
        /// <param name="_sectionName">The section, if any, from which we wish to retrieve a vector in the list named <paramref name="_key"/>.</param>
        /// <returns>The vector at index <paramref name="_index"/> in the list named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</returns>
        static orxVECTOR __fastcall GetListVector(const std::string _key, const int _index, const std::string _sectionName = "");
        /// <summary>Gets a string value named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</summary>
        /// <param name="_key">The name of the string we wish to retrieve from config.</param>
        /// <param name="_sectionName">The section, if any, from which we wish to retrieve <paramref name="_key"/>.</param>
        /// <returns>The string value named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</returns>
        static const std::string __fastcall GetString(const std::string _key, const std::string _sectionName = "");
        /// <summary>Gets the string at index <paramref name="_index"/> in the list named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</summary>
        /// <param name="_key">The name of the list from which we wish to retrieve a string in config.</param>
        /// <param name="_index">The index of the list named <paramref name="_key"/> at which we wish to retrieve a string.</param>
        /// <param name="_sectionName">The section, if any, from which we wish to retrieve a string in the list named <paramref name="_key"/>.</param>
        /// <returns>The string at index <paramref name="_index"/> in the list named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</returns>
        static const std::string __fastcall GetListString(const std::string _key, const int _index, const std::string _sectionName = "");
        /// <summary>Gets the number of items held by the list named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</summary>
        /// <param name="_key">The name of the list.</param>
        /// <param name="_sectionName">The section, if any, in which we expect to find the list named <paramref name="_key"/>.</param>
        /// <returns>The number of itemse held by the list named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) from config.</returns>
        static const int __fastcall GetListCount(const std::string _key, const std::string _sectionName = "");
        /// <summary>Sets a bool value named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) to <paramref name="_value"/>.</summary>
        /// <param name="_key">The name of the bool we wish to set in config.</param>
        /// <param name="_value">The value to which we're setting the bool named <paramref name="_key"/>.</param>
        /// <param name="_sectionName">The section, if any, in which we wish to set <paramref name="_key"/>.</param>
        /// <returns>An orxSTATUS code.</returns>
        static orxSTATUS __fastcall SetBool(const std::string _key, orxBOOL _value, const std::string _sectionName = "");
        /// <summary>Sets a float value named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) to <paramref name="_value"/>.</summary>
        /// <param name="_key">The name of the float we wish to set in config.</param>
        /// <param name="_value">The value to which we're setting the float named <paramref name="_key"/>.</param>
        /// <param name="_sectionName">The section, if any, in which we wish to set <paramref name="_key"/>.</param>
        /// <returns>An orxSTATUS code.</returns>
        static orxSTATUS __fastcall SetFloat(const std::string _key, orxFLOAT _value, const std::string _sectionName = "");
        /// <summary>Sets a vector value named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) to <paramref name="_value"/>.</summary>
        /// <param name="_key">The name of the vector we wish to set in config.</param>
        /// <param name="_value">The value to which we're setting the vector named <paramref name="_key"/>.</param>
        /// <param name="_sectionName">The section, if any, in which we wish to set <paramref name="_key"/>.</param>
        /// <returns>An orxSTATUS code.</returns>
        static orxSTATUS __fastcall SetVector(const std::string _key, const orxVECTOR *_value, const std::string _sectionName = "");
        /// <summary>Sets a string value named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) to <paramref name="_value"/>.</summary>
        /// <param name="_key">The name of the string we wish to set in config.</param>
        /// <param name="_value">The value to which we're setting the string named <paramref name="_key"/>.</param>
        /// <param name="_sectionName">The section, if any, in which we wish to set <paramref name="_key"/>.</param>
        /// <returns>An orxSTATUS code.</returns>
        static orxSTATUS __fastcall SetString(const std::string _key, const std::string _value, const std::string _sectionName = "");
        /// <summary>Sets a list of strings named <paramref name="_key"/> (and optionally associated with the section named <paramref name="_sectionName"/>) to <paramref name="_list"/>.</summary>
        /// <param name="_key">The name of the string list we wish to set in config.</param>
        /// <param name="_list">The value to which we're setting the string list named <paramref name="_key"/>.</param>
        /// <param name="_numValues">The number of values in <paramref name="_list"/>.</param>
        /// <param name="_sectionName">The section, if any, in which we wish to set <paramref name="_key"/>.</param>
        /// <returns>An orxSTATUS code.</returns>
        static orxSTATUS __fastcall SetListString(const std::string _key, const char **_list, int _numValues, const std::string _sectionName = "");
        /// <summary>Sets the ScrollMod's position in terms of parent space, such that 0.5 represents the ScrollMod's maximum bound and -0.5 represents its minimum bound.</summary>
        /// <param name="_position">The position to which we'll set the ScrollMod, in terms of parent space.</param>
        void __fastcall SetParentSpacePosition(const orxVECTOR &_position);
        /// <summary>A more object-oriented version of ORX's orxObject_GetActiveTime function.</summary>
        /// <returns>The amount of time that the ScrollMod has been alive, taking into account the ScrollMod's clock multiplier and periods of pause.</returns>
        float __fastcall GetActiveTime();
        /// <summary>A more object-oriented version of ORX's orxObject_GetOwner() function.</summary>
        /// <returns>The orxSTRUCTURE which is the owner of the ScrollMod, or NULL if there is none.</returns>
        orxSTRUCTURE *__fastcall GetOwner();
        /// <summary>A more object-oriented version of ORX's orxObject_GetParent() function.</summary>
        /// <returns>The orxSTRUCTURE which is the parent of the ScrollMod, or NULL if there is none.</returns>
        orxSTRUCTURE *__fastcall GetParent();
        /// <summary>A more object-oriented version of ORX's orxObject_SetOwner() function.</summary>
        /// <param name="_owner">The ScrollObject we wish to be the owner of the ScrollMod.</param>
        void __fastcall SetOwner(const ScrollObject *_owner);
        /// <summary>A more object-oriented version of ORX's orxObject_SetParent() function.</summary>
        /// <param name="_parent">The ScrollObject we wish to be the parent of the ScrollMod.</param>
        /// <returns>The orxSTATUS returned by the wrapped ORX function.</returns>
        orxSTATUS __fastcall SetParent(const ScrollObject *_parent);
        /// <summary>Gets the SelfFlags attribute of <paramref name="_part"/>.</summary>
        /// <param name="_part">The body part of which we're getting the SelfFlags attribute.</param>
        /// <returns>The SelfFlags attribute of <paramref name="_part"/>.</returns>
        orxU16 __fastcall GetSelfFlags(const orxBODY_PART *_part);
        /// <summary>Gets the CheckMask attribute of <paramref name="_part"/>.</summary>
        /// <param name="_part">The body part of which we're getting the CheckMask attribute.</param>
        /// <returns>The CheckMask attribute of <paramref name="_part"/>.</returns>
        orxU16 __fastcall GetCheckMask(const orxBODY_PART *_part);
        /// <summary>Sets the SelfFlags attribute of <paramref name="_part"/> to <paramref name="_selfFlags"/>.</summary>
        /// <param name="_part">The body part whose SelfFlags attribute we're setting to <paramref name="_selfFlags"/>.</param>
        /// <param name="_selfFlags">The value to which we're setting <paramref name="_part"/>'s SelfFlags attribute.</param>
        /// <returns>An orxSTATUS code.</returns>
        orxSTATUS __fastcall SetSelfFlags(orxBODY_PART *_part, orxU16 _selfFlags);
        /// <summary>Sets the CheckMask attribute of <paramref name="_part"/> to <paramref name="_checkMask"/>.</summary>
        /// <param name="_part">The body part whose CheckMask attribute we're setting to <paramref name="_checkMask"/>.</param>
        /// <param name="_selfFlags">The value to which we're setting <paramref name="_part"/>'s CheckMask attribute.</param>
        /// <returns>An orxSTATUS code.</returns>
        orxSTATUS __fastcall SetCheckMask(orxBODY_PART *_part, orxU16 _checkMask);
        /// <summary>Returns whether or not the ScrollMod has reached its existence time limit (if it has one).</summary>
        /// <returns>A bool representing whether or not the ScrollMod has reached its existence time limit (if it has one).</returns>
        const bool __fastcall IsPendingDestruction();
        /// <summary>Returns the axis-aligned bounding box associated with the ScrollMod.</summary>
        /// <returns>The axis-aligned bounding box associated with the ScrollMod.</returns>
        orxAABOX __fastcall GetAABB();
        /// <summary>Returns the orxSTRUCTURE, if it exists, that matches <paramref name="_structureID"/> and is associated with the ScrollMod.</summary>
        /// <param name="_structureID">The orxSTRUCTURE_ID of the structure we wish to locate within the ScrollMod.</param>
        /// <returns>The orxSTRUCTURE, if it exists, which matches <paramref name="_structureID"/> and is associated with the ScrollMod.</returns>
        orxSTRUCTURE *GetStructure(orxSTRUCTURE_ID _structureID);
        /// <summary>Returns the ScrollMod's associated orxBODY structure, or NULL if there is none.</summary>
        /// <returns>The ScrollMod's associated orxBODY structure, or NULL if there is none.</returns>
        orxBODY *GetBody();
        /// <summary>Adds the orxBODY_PART associated with <paramref name="_partName"/> in config to the orxBODY structure associated with the ScrollMod.</summary>
        /// <returns>A pointer to the orxBODY_PART just added.</returns>
        orxBODY_PART *__fastcall AddBodyPartByName(const std::string _partName);
        /// <summary>Removes the orxBODY_PART associated with <paramref name="_partName"/> in config from the orxBODY structure associated with the ScrollMod.</summary>
        /// <returns>An orxSTATUS code.</returns>
        orxSTATUS __fastcall RemoveBodyPartByName(const std::string _partName);
        /// <summary>A more object-oriented version of ORX's orxObject_AddUniqueDelayedFX function.</summary>
        /// <param name="_fxName">The config name of the FX we wish to add.</param>
        /// <param name="_delay">Delay time.</param>
        /// <returns>An orxSTATUS code.</returns>
        orxSTATUS __fastcall AddUniqueFX(const std::string _fxName, const float &_delay = 0);
        /// <summary>Returns whether the ScrollMod's position is currently outside of the bounds of the passed camera's frustum.</summary>
        /// <param name="_cameraName">The name of the camera whose frustum we're checking.</param>
        /// <returns>Whether the ScrollMod's position is currently outside of the bounds of the passed camera's frustum.</returns>
        const bool __fastcall IsOffCamera(std::string _cameraName);
        /// <summary>Returns the number of children the ScrollMod has.</summary>
        /// <returns>The number of children the ScrollMod has.</returns>
        int __fastcall GetNumChildren();
        /// <summary>Returns a pointer to the ScrollObject, if it exists, which is a child of the ScrollMod, and which has the name <paramref name="_childName"/>.</summary>
        /// <param name="_childName">The name by which we're searching for a child of the ScrollMod.</param>
        /// <returns>A pointer to the ScrollMod, if it exists, which is a child of the ScrollMod, and which has the name <paramref name="_childName"/>.</returns>
        ScrollObject *GetChildByName(const std::string _childName);
        /// <summary>Returns a pointer to the ScrollObject, if it exists, which is a child of the ScrollMod, and which has a name contained within <paramref name="_childNameList"/>.</summary>
        /// <param name="_childName">The names by which we're searching for a child of the ScrollMod.</param>
        /// <returns>A pointer to the ScrollObject, if it exists, which is a child of the ScrollMod, and which has a name contained within <paramref name="_childNameList"/>.</returns>
        ScrollObject *GetChildByName(const std::vector<std::string> &_childNameList);
        /// <summary>Returns the world's gravity.</summary>
        /// <returns>The world's gravity.</returns>
        orxVECTOR GetWorldGravity();
        /// <summary>Returns a pointer to the body part, if it exists, which is a part of the ScrollMod's body, and which has the name <paramref name="_partName"/>.</summary>
        /// <param name="_partName">The name by which we're searching for a body part which is a part of the ScrollMod's body.</param>
        /// <returns>A pointer to the body part, if it exists, which is a part of the ScrollMod's body, and which has the name <paramref name="_partName"/>.</returns>
        orxBODY_PART *GetBodyPartByName(const std::string _partName);
        /// <summary>Sets the ScrollMod's target animation to <paramref name="_animName"/>.</summary>
        /// <param name="_animName">The animation to which we're setting the ScrollMod's target animation.</param>
        void SetTargetAnim(const std::string _animName);
        /// <summary>Sets the ScrollMod's custom gravity to <paramref name="_customGravity"/>.</summary>
        /// <param name="_customGravity">The gravity to which we're setting the ScrollMod's custom gravity.</param>
        orxSTATUS __fastcall SetCustomGravity(orxVECTOR &_customGravity);
        /// <summary>Sets the body part of the ScrollMod associated with <paramref name="_partName"/>'s solidity to the bool value represented by <paramref name="_bSolid"/>.</summary>
        /// <param name="_partName">The name of the body part whose solidity we wish to set.</param>
        /// <param name="_bSolid">The bool value representing the solidity we wish to set.</param>
        void SetBodyPartSolid(const std::string _partName, const orxBOOL &_bSolid);
        /// <summary>Moves The ScrollMod from its current position to <paramref name="_destination"/> at a speed of <paramref name="_speed"/>, and with a deceleration distance of <paramref name="decelerationDistance"/>.</summary>
        /// <param name="_destination">The position to which we're moving the ScrollMod.</param>
        /// <param name="_speed">The speed at which we're moving the ScrollMod to <paramref name="_destination"/>.</param>
        /// <param name="_decelerationDistance">The distance between the ScrollMod and <paramref name="_destination"/> at which the movement of the ScrollMod will begin to slow, so as to avoid "jittering" upon reaching the destination.</param>
        void MoveTo(const orxVECTOR &_destination, const float &_speed, const float &_decelerationDistance);
        /// <summary>Destroys the ScrollMod immediately.</summary>
        void Destroy();
    };
}
