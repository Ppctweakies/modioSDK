#include "c/methods/callbacks/MediaCallbacks.h"

std::map< u32, AddModLogoParams* > add_mod_logo_callbacks;
std::map< u32, AddModImagesParams* > add_mod_images_callbacks;
std::map< u32, AddModYoutubeLinksParams* > add_mod_youtube_links_callbacks;
std::map< u32, AddModSketchfabLinksParams* > add_mod_sketchfab_links_callbacks;
std::map< u32, DeleteModImagesParams* > delete_mod_images_callbacks;
std::map< u32, DeleteModYoutubeLinksParams* > delete_mod_youtube_links_callbacks;
std::map< u32, DeleteModSketchfabLinksParams* > delete_mod_sketchfab_links_callbacks;

void modioOnAddModLogo(u32 call_number, u32 response_code, nlohmann::json response_json)
{
  ModioResponse response;
  modioInitResponse(&response, response_json);
  response.code = response_code;

  add_mod_logo_callbacks[call_number]->callback(add_mod_logo_callbacks[call_number]->object, response);
  delete add_mod_logo_callbacks[call_number];
  add_mod_logo_callbacks.erase(call_number);

  modioFreeResponse(&response);
}

void modioOnAddModImages(u32 call_number, u32 response_code, nlohmann::json response_json)
{
  ModioResponse response;
  modioInitResponse(&response, response_json);
  response.code = response_code;

  add_mod_images_callbacks[call_number]->callback(add_mod_images_callbacks[call_number]->object, response);
  delete add_mod_images_callbacks[call_number];
  add_mod_images_callbacks.erase(call_number);

  modioFreeResponse(&response);
}

void modioOnAddModYoutubeLinks(u32 call_number, u32 response_code, nlohmann::json response_json)
{
  ModioResponse response;
  modioInitResponse(&response, response_json);
  response.code = response_code;

  add_mod_youtube_links_callbacks[call_number]->callback(add_mod_youtube_links_callbacks[call_number]->object, response);
  delete add_mod_youtube_links_callbacks[call_number];
  add_mod_youtube_links_callbacks.erase(call_number);

  modioFreeResponse(&response);
}

void modioOnAddModSketchfabLinks(u32 call_number, u32 response_code, nlohmann::json response_json)
{
  ModioResponse response;
  modioInitResponse(&response, response_json);
  response.code = response_code;

  add_mod_sketchfab_links_callbacks[call_number]->callback(add_mod_sketchfab_links_callbacks[call_number]->object, response);
  delete add_mod_sketchfab_links_callbacks[call_number];
  add_mod_sketchfab_links_callbacks.erase(call_number);

  modioFreeResponse(&response);
}

void modioOnDeleteModImages(u32 call_number, u32 response_code, nlohmann::json response_json)
{
  ModioResponse response;
  modioInitResponse(&response, response_json);
  response.code = response_code;

  delete_mod_images_callbacks[call_number]->callback(delete_mod_images_callbacks[call_number]->object, response);
  delete delete_mod_images_callbacks[call_number];
  delete_mod_images_callbacks.erase(call_number);

  modioFreeResponse(&response);
}

void modioOnDeleteModYoutubeLinks(u32 call_number, u32 response_code, nlohmann::json response_json)
{
  ModioResponse response;
  modioInitResponse(&response, response_json);
  response.code = response_code;

  delete_mod_youtube_links_callbacks[call_number]->callback(delete_mod_youtube_links_callbacks[call_number]->object, response);
  delete delete_mod_youtube_links_callbacks[call_number];
  delete_mod_youtube_links_callbacks.erase(call_number);

  modioFreeResponse(&response);
}

void modioOnDeleteModSketchfabLinks(u32 call_number, u32 response_code, nlohmann::json response_json)
{
  ModioResponse response;
  modioInitResponse(&response, response_json);
  response.code = response_code;

  delete_mod_sketchfab_links_callbacks[call_number]->callback(delete_mod_sketchfab_links_callbacks[call_number]->object, response);
  delete delete_mod_sketchfab_links_callbacks[call_number];
  delete_mod_sketchfab_links_callbacks.erase(call_number);

  modioFreeResponse(&response);
}