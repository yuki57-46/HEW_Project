#pragma once

#include <stdio.h>
#include "Model.h"

// 表示するモデルのパス
#define MODEL_PATH ("Assets/Model/mixamo/Running.fbx")
//#define MODEL_PATH ("Assets/Model/mixamo/sotai_pinch.fbx")

// 表示させるアニメーションのパス
// アニメーションとモデルが分岐している場合↓
#define ANIME_PATH ("Assets/Model/Animation/Running.fbx")
//#define ANIME_PATH ("Assets/Model/mixamo/sotai_pinch.fbx")

// 表示する大きさ
// 1.0fでFBXのサイズ通り
//#define MODEL_SCALE (0.6f)		// 黒子用 
#define MODEL_SCALE (0.01f)		// 配布用（ランニング）

// モデルの表示の向き
// Model::None  でDirectXの標準の表示の仕方
// Model::XFlip でMayaの表示の仕方
// Model::ZFlip でDirectXの表示の仕方(Mayaから180度回転した状態)
#define MODEL_FLIP (Model::XFlip)

