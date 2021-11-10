#include "TextureFont.h"

using namespace MelLib;
std::unordered_map < std::string, std::unique_ptr<TextureFont >> TextureFont::pTextureFonts;

bool TextureFont::CreateSprite(const std::string& path, const Value2<UINT> lineNum, const std::string& name)
{
    bool result = Texture::CreateSprite(path, name);
    if (!result)return false;

    pTextureFonts.emplace(name, std::make_unique<TextureFont>());
    pTextureFonts[name]->pTexture = Texture::Get(name);
    pTextureFonts[name]->fonsLineNum = lineNum;

    return true;
}

void TextureFont::Delete(const std::string& name)
{
    Texture::Delete(name);
    pTextureFonts.erase(name);
}
