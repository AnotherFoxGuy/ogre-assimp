/*
-----------------------------------------------------------------------------
This source file is part of
                                    _
  ___   __ _ _ __ ___  __ _ ___ ___(_)_ __ ___  _ __
 / _ \ / _` | '__/ _ \/ _` / __/ __| | '_ ` _ \| '_ \
| (_) | (_| | | |  __/ (_| \__ \__ \ | | | | | | |_) |
 \___/ \__, |_|  \___|\__,_|___/___/_|_| |_| |_| .__/
       |___/                                   |_|

For the latest info, see https://bitbucket.org/jacmoe/ogreassimp

Copyright (c) 2011 Jacob 'jacmoe' Moen

Licensed under the MIT license:

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __AssimpLoader_h__
#define __AssimpLoader_h__

#include <OgreMesh.h>

#include <assimp/scene.h>

namespace Assimp
{
    class Importer;
}

class AssimpLoader
{
public:
    enum LoaderParams
    {
        // 3ds max exports the animation over a longer time frame than the animation actually plays for
        // this is a fix for that
        LP_CUT_ANIMATION_WHERE_NO_FURTHER_CHANGE = 1<<0,

        // Quiet mode - don't output anything
        LP_QUIET_MODE = 1<<1
    };

    struct Options
    {
        float animationSpeedModifier;
        int params;
        Ogre::String customAnimationName;
        float maxEdgeAngle;

        Options() : animationSpeedModifier(1), params(0), maxEdgeAngle(30) {}
    };

    AssimpLoader();
    virtual ~AssimpLoader();

    bool load(const Ogre::String& source, Ogre::Mesh* mesh, Ogre::SkeletonPtr& skeletonPtr,
              const Options& options = Options());

    bool load(const Ogre::DataStreamPtr& source, const Ogre::String& type, Ogre::Mesh* mesh,
              Ogre::SkeletonPtr& skeletonPtr, const Options& options = Options());

private:
    bool _load(const char* name, Assimp::Importer& importer, Ogre::Mesh* mesh, Ogre::SkeletonPtr& skeletonPtr, const Options& options);
    bool createSubMesh(const Ogre::String& name, int index, const aiNode* pNode, const aiMesh *mesh, const aiMaterial* mat, Ogre::Mesh* mMesh, Ogre::AxisAlignedBox& mAAB);
    Ogre::MaterialPtr createMaterial(int index, const aiMaterial* mat);
    void grabNodeNamesFromNode(const aiScene* mScene,  const aiNode* pNode);
    void grabBoneNamesFromNode(const aiScene* mScene,  const aiNode* pNode);
    void computeNodesDerivedTransform(const aiScene* mScene,  const aiNode *pNode, const aiMatrix4x4 accTransform);
    void createBonesFromNode(const aiScene* mScene,  const aiNode* pNode);
    void createBoneHiearchy(const aiScene* mScene,  const aiNode *pNode);
    void loadDataFromNode(const aiScene* mScene,  const aiNode *pNode, Ogre::Mesh* mesh);
    void markAllChildNodesAsNeeded(const aiNode *pNode);
    void flagNodeAsNeeded(const char* name);
    bool isNodeNeeded(const char* name);
    void parseAnimation (const aiScene* mScene, int index, aiAnimation* anim);
    typedef std::map<Ogre::String, bool> boneMapType;
    boneMapType boneMap;
    //aiNode* mSkeletonRootNode;
    int mLoaderParams;

    Ogre::String mCustomAnimationName;

    typedef std::map<Ogre::String, const aiNode*> BoneNodeMap;
    BoneNodeMap mBoneNodesByName;

    typedef std::map<Ogre::String, const aiBone*> BoneMap;
    BoneMap mBonesByName;

    typedef std::map<Ogre::String, aiMatrix4x4> NodeTransformMap;
    NodeTransformMap mNodeDerivedTransformByName;

    Ogre::SkeletonPtr mSkeleton;

    static int msBoneCount;

    bool mQuietMode;
    Ogre::Real mTicksPerSecond;
    Ogre::Real mAnimationSpeedModifier;
};

#endif // __AssimpLoader_h__
