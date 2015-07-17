///
/// PlanetData.h
/// Seed of Andromeda
///
/// Created by Benjamin Arnold on 5 Feb 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Structs for planetary data
///

#pragma once

#ifndef PlanetData_h__
#define PlanetData_h__

#include <Vorb/VorbPreDecl.inl>
#include <Vorb/graphics/GLProgram.h>
#include <Vorb/graphics/ImageIO.h>
#include <Vorb/graphics/Texture.h>
#include <Vorb/io/Keg.h>

#include "Noise.h"
#include "Biome.h"

DECL_VG(class GLProgram; class BitmapResource);

struct LiquidColorKegProperties {
    nString colorPath = "";
    nString texturePath = "";
    ColorRGB8 tint = ColorRGB8(255, 255, 255);
    f32 depthScale = 1000.0f;
    f32 freezeTemp = -1.0f;
};
KEG_TYPE_DECL(LiquidColorKegProperties);

struct TerrainColorKegProperties {
    nString colorPath = "";
    nString grassTexturePath = "";
    nString rockTexturePath = "";
    ColorRGB8 tint = ColorRGB8(255, 255, 255);
};
KEG_TYPE_DECL(TerrainColorKegProperties);

// Must match var names for TreeFruitProperties
struct FruitKegProperties {
    nString flora = "";
    f32v2 chance = f32v2(0.0f);
};

// Must match var names for TreeLeafProperties
struct LeafKegProperties {
    TreeLeafType type = TreeLeafType::NONE;
    FruitKegProperties fruitProps;
    // Union based on type
    union {
        UNIONIZE(struct {
            ui32v2 radius;
        } round;);
        UNIONIZE(struct {
            ui32v2 width;
            ui32v2 height;
        } cluster;);
        UNIONIZE(struct {
            ui32v2 thickness;
        } pine;);
        UNIONIZE(struct {
            i32v2 lengthMod;
            i32v2 curlLength;
            i32v2 capThickness;
            i32v2 gillThickness;
        } mushroom;);
    };
    // Don't put strings in unions
    nString roundBlock = "";
    nString clusterBlock = "";
    nString pineBlock = "";
    nString mushGillBlock = "";
    nString mushCapBlock = "";
};

// Must match var names for TreeBranchProperties
struct BranchKegProperties {
    ui32v2 coreWidth = ui32v2(0);
    ui32v2 barkWidth = ui32v2(0);
    ui32v2 length = ui32v2(0);
    f32v2 branchChance = f32v2(0.0f);
    f32v2 angle = f32v2(0.0f);
    ui32v2 segments[2];
    f32 endSizeMult = 0.0f;
    nString coreBlock = "";
    nString barkBlock = "";
    FruitKegProperties fruitProps;
    LeafKegProperties leafProps;
};

// Must match var names for TreeTrunkProperties
struct TrunkKegProperties {
    f32 loc = 0.0f;
    ui32v2 coreWidth;
    ui32v2 barkWidth;
    f32v2 branchChance;
    i32v2 slope[2];
    nString coreBlock = "";
    nString barkBlock = "";
    FruitKegProperties fruitProps;
    LeafKegProperties leafProps;
    BranchKegProperties branchProps;
};

// Must match var names for TreeData
struct TreeKegProperties {
    nString id = "";
    ui32v2 heightRange = ui32v2(0, 0);
    std::vector<TrunkKegProperties> trunkProps;
};

// Info about what blocks a planet needs
struct PlanetBlockInitInfo {
    std::map<const Biome*, std::vector<BiomeFloraKegProperties>> biomeFlora;
    std::map<const Biome*, std::vector<BiomeTreeKegProperties>> biomeTrees;
    std::vector<TreeKegProperties> trees;
    std::vector<nString> blockLayerNames;
    std::vector<nString> floraBlockNames;
    nString liquidBlockName = "";
    nString surfaceBlockName = "";
};

struct PlanetGenData {
    vg::Texture terrainColorMap = 0;
    vg::Texture liquidColorMap = 0;
    vg::Texture grassTexture = 0;
    vg::Texture rockTexture = 0;
    vg::Texture liquidTexture = 0;
    vg::BitmapResource terrainColorPixels;
    vg::BitmapResource liquidColorPixels;
    color3 liquidTint = color3(255, 255, 255);
    color3 terrainTint = color3(255, 255, 255);
    f32 liquidDepthScale = 1000.0f;
    f32 liquidFreezeTemp = -1.0f;
    f32 tempLatitudeFalloff = 0.0f;
    f32 tempHeightFalloff = 0.0f;
    f32 humLatitudeFalloff = 0.0f;
    f32 humHeightFalloff = 0.0f;
    PlanetBlockInitInfo blockInfo;
    std::vector<BlockLayer> blockLayers;
    ui32 liquidBlock = 0;
    ui32 surfaceBlock = 0;
    f64 radius = 0.0;

    /************************************************************************/
    /* Base Noise                                                           */
    /************************************************************************/
    NoiseBase baseTerrainFuncs;
    NoiseBase tempTerrainFuncs;
    NoiseBase humTerrainFuncs;

    /************************************************************************/
    /* Flora and Trees                                                      */
    /************************************************************************/
    std::vector<FloraData> flora;
    std::map<nString, ui32> floraMap;
    std::vector<NTreeType> trees;
    std::map<nString, ui32> treeMap;

    /************************************************************************/
    /* Biomes                                                               */
    /************************************************************************/
    const Biome* baseBiomeLookup[BIOME_MAP_WIDTH][BIOME_MAP_WIDTH];
    std::vector<BiomeInfluence> baseBiomeInfluenceMap[BIOME_MAP_WIDTH][BIOME_MAP_WIDTH];
    std::vector<Biome> biomes; ///< Biome object storage. DON'T EVER RESIZE AFTER GEN.

    nString terrainFilePath;
};

#endif // PlanetData_h__
