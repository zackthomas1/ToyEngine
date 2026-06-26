#pragma once

enum class AssetType
{
    None        = 0,
    Scene       = 1,
    Model       = 2,
    Mesh        = 3,
    Material    = 4,
    Texture     = 5,
    EnvMap      = 6,
};

struct Asset
{

};

struct ModelAsset : Asset
{

};

class AssetManager {
public:
    AssetManager() = default;

    void Import(AssetType type, const char* path);
    Asset Get(const char* name);
    void Delete();

private:


};