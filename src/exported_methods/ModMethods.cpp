#include "exported_methods/ModMethods.h"

extern "C"
{
  struct GetModsParams
  {
    void (*callback)(int response_code, char* message, Mod* mods, int mods_size);
  };

  struct AddModParams
  {
    void (*callback)(int response_code, char* message, Mod* mod);
  };

  struct DeleteModParams
  {
    Mod* mod;
    void (*callback)(int response_code, char* message, Mod* mod);
  };

/*
  struct DownloadImageParams
  {
    Mod* mod;
    void (*)(int, char*, Mod*, char*) callback;
  };

  struct DownloadImagesParams
  {
    Mod* mod;
    int image_amount;
    vector<string> images;
    void (*)(int, char*, Mod*, char**, int) callback;
  };

  struct DownloadModfileParams
  {
    Mod* mod;
    string destination_path;
    void (*)(int, char*, Mod*, string) callback;
  };
*/

  map< int,AddModParams* > add_mod_callback;
  map< int,DeleteModParams* > delete_mod_callbacks;
  map< int,GetModsParams* > get_mods_callbacks;

/*
  map< int, DownloadImageParams* > download_image_callbacks;
  map< int, DownloadImagesParams* > download_images_callbacks;
  map< int, DownloadModfileParams* > download_modfile_callbacks;
*/

  void onGetMods(int call_number, int response_code, string message, json response)
  {
    Mod* mods = NULL;
    int mods_size = 0;
    if(response_code == 200)
    {
      mods_size = (int)response["data"].size();
      mods = new Mod[mods_size];
      for(int i=0;i<mods_size;i++)
      {
        initMod(&mods[i], response["data"][i]);
      }
    }
    get_mods_callbacks[call_number]->callback(response_code, (char*)message.c_str(), mods, mods_size);
    get_mods_callbacks.erase(call_number);
  }

  void getMods(Filter* filter, void (*callback)(int response_code, char* message, Mod* mods, int mods_size))
  {
    string filter_string = getFilterString(filter);
    vector<string> headers;
    headers.push_back("Authorization: Bearer " + modio::ACCESS_TOKEN);
    string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods?" + filter_string + "&shhh=secret";

    int call_number = modio::curlwrapper::getCallCount();
    modio::curlwrapper::advanceCallCount();

    get_mods_callbacks[call_number] = new GetModsParams;
    get_mods_callbacks[call_number]->callback = callback;

    std::thread get_mods_thread(modio::curlwrapper::get, call_number, url, headers, &onGetMods);
    get_mods_thread.detach();
  }

  void onModAdded(int call_number, int response_code, string message, json response)
  {
    Mod* mod = new Mod;
    initMod(mod, response);
    add_mod_callback[call_number]->callback(response_code, (char*)message.c_str(), mod);
    add_mod_callback.erase(call_number);
  }

  void editMod(Mod* mod, ModHandler* add_mod_handler, void (*callback)(int response_code, char* message, Mod* mod))
  {
    vector<string> headers;
    headers.push_back("Authorization: Bearer " + modio::ACCESS_TOKEN);

    int call_number = modio::curlwrapper::getCallCount();
    modio::curlwrapper::advanceCallCount();

    add_mod_callback[call_number] = new AddModParams;
    add_mod_callback[call_number]->callback = callback;

    string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods/" + modio::toString(mod->id);

    std::thread email_exchage_thread(modio::curlwrapper::put, call_number, url, headers, add_mod_handler->curlform_copycontents, &onModAdded);
    email_exchage_thread.detach();
  }

  void addMod(ModHandler* add_mod_handler, void (*callback)(int response_code, char* message, Mod* mod))
  {
    vector<string> headers;
    headers.push_back("Authorization: Bearer " + modio::ACCESS_TOKEN);

    int call_number = modio::curlwrapper::getCallCount();
    modio::curlwrapper::advanceCallCount();

    add_mod_callback[call_number] = new AddModParams;
    add_mod_callback[call_number]->callback = callback;

    string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods";

    std::thread add_mod_thread(modio::curlwrapper::postForm, call_number, url, headers, add_mod_handler->curlform_copycontents, add_mod_handler->curlform_files, &onModAdded);
    add_mod_thread.detach();
  }

  void onModDeleted(int call_number, int response_code, string message, json response)
  {
    Mod* mod = new Mod;
    initMod(mod, response);
    delete_mod_callbacks[call_number]->callback(response_code, (char*)message.c_str(), mod);
    delete_mod_callbacks.erase(call_number);
  }

  void MODIO_DLL deleteMod(Mod* mod, void (*callback)(int response_code, char* message, Mod* mod))
  {
    vector<string> headers;
    headers.push_back("Authorization: Bearer " + modio::ACCESS_TOKEN);

    int call_number = modio::curlwrapper::getCallCount();
    modio::curlwrapper::advanceCallCount();

    delete_mod_callbacks[call_number] = new DeleteModParams;
    delete_mod_callbacks[call_number]->callback = callback;

    string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods/" + modio::toString(mod->id);

    std::thread delete_mod_thread(modio::curlwrapper::deleteCall, call_number, url, headers, &onModDeleted);
    delete_mod_thread.detach();
  }

/*
  void onImageDownloaded(int call_number, int response_code, string message, string url, string path)
  {
    download_image_callbacks[call_number]->callback(response_code, message, download_image_callbacks[call_number]->mod, path);
    download_image_callbacks.erase(call_number);
  }

  void downloadModLogoThumbnail(Mod *mod, function< void(int response_code, string message, Mod* mod, string path) > callback)
  {
    string file_path = string(getModIODirectory() + "images/") + toString(mod->id) + "_mod_logo_thumb.png";

    int call_number = curlwrapper::getCallCount();
    curlwrapper::advanceCallCount();

    download_image_callbacks[call_number] = new DownloadImageParams;
    download_image_callbacks[call_number]->mod = mod;
    download_image_callbacks[call_number]->callback = callback;

    std::thread download_image_thread(curlwrapper::download, call_number, mod->logo->thumbnail, file_path, &onImageDownloaded);

    download_image_thread.detach();
  }

  void downloadModLogoFull(Mod *mod, function< void(int response_code, string message, Mod*mod, string path) > callback)
  {
    string file_path = string(getModIODirectory() + "images/") + toString(mod->id) + "_mod_logo_full.png";

    int call_number = curlwrapper::getCallCount();
    curlwrapper::advanceCallCount();

    download_image_callbacks[call_number] = new DownloadImageParams;
    download_image_callbacks[call_number]->mod = mod;
    download_image_callbacks[call_number]->callback = callback;

    std::thread download_image_thread(curlwrapper::download, call_number, mod->logo->full, file_path, &onImageDownloaded);

    download_image_thread.detach();
  }

  void onImageFromVectorDownloaded(int call_number, int response_code, string message, string url, string path)
  {
    download_images_callbacks[call_number]->images.push_back(path);
    if((int)download_images_callbacks[call_number]->images.size() == download_images_callbacks[call_number]->image_amount)
    {
      download_images_callbacks[call_number]->callback(response_code, message, download_images_callbacks[call_number]->mod, download_images_callbacks[call_number]->images);
    }
    download_images_callbacks.erase(call_number);
  }

  void downloadModMediaImagesThumbnail(Mod *mod, function< void(int response_code, string message, Mod* mod, vector<string> paths) > callback)
  {
    DownloadImagesParams* download_images_params = new DownloadImagesParams;
    download_images_params->mod = mod;
    download_images_params->image_amount = mod->media->images.size();
    download_images_params->callback = callback;

    for(int i=0; i<(int)mod->media->images.size();i++)
    {
      int call_number = curlwrapper::getCallCount();
      curlwrapper::advanceCallCount();
      download_images_callbacks[call_number] = download_images_params;
      createDirectory(string(getModIODirectory() + "images/") + toString(mod->id) + "_mod_media/");
      string file_path = string(getModIODirectory() + "images/") + toString(mod->id) + "_mod_media/" + toString(i) + "_image_thumb.png";
      std::thread download_image_thread(curlwrapper::download, call_number, mod->media->images[i]->thumbnail, file_path, &onImageFromVectorDownloaded);
      download_image_thread.detach();
    }
  }

  void downloadModMediaImagesFull(Mod *mod, function< void(int response_code, string message, Mod* mod, vector<string> paths) > callback)
  {
    DownloadImagesParams* download_images_params = new DownloadImagesParams;
    download_images_params->mod = mod;
    download_images_params->image_amount = mod->media->images.size();
    download_images_params->callback = callback;

    for(int i=0; i<(int)mod->media->images.size();i++)
    {
      int call_number = curlwrapper::getCallCount();
      curlwrapper::advanceCallCount();
      download_images_callbacks[call_number] = download_images_params;
      createDirectory(string(getModIODirectory() + "images/") + toString(mod->id) + "_mod_media/");
      string file_path = string(getModIODirectory() + "images/") + toString(mod->id) + "_mod_media/" + toString(i) + "_image_full.png";
      std::thread download_image_thread(curlwrapper::download, call_number, mod->media->images[i]->full, file_path, &onImageFromVectorDownloaded);
      download_image_thread.detach();
    }
  }

  void onModfileDownloaded(int call_number, int response_code, string message, string url, string path)
  {
    string destintation_path = download_modfile_callbacks[call_number]->destination_path;
    createDirectory(destintation_path);
    minizipwrapper::extract(path, destintation_path);
    removeFile(path);
    download_modfile_callbacks[call_number]->callback(response_code, message, download_modfile_callbacks[call_number]->mod, path);
    download_modfile_callbacks.erase(call_number);
  }

  void installMod(Mod *mod, string destination_path, function< void(int response_code, string message, Mod* mod, string path) > callback)
  {
    string file_path = string(getModIODirectory() + "tmp/") + toString(mod->modfile->id) + "_modfile.zip";

    int call_number = curlwrapper::getCallCount();
    curlwrapper::advanceCallCount();

    download_modfile_callbacks[call_number] = new DownloadModfileParams;
    download_modfile_callbacks[call_number]->mod = mod;
    download_modfile_callbacks[call_number]->destination_path = destination_path;
    download_modfile_callbacks[call_number]->callback = callback;

    std::thread download_thread(curlwrapper::download, call_number, mod->modfile->download + "?shhh=secret", file_path, &onModfileDownloaded);
    download_thread.detach();
  }
*/

}
