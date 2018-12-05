#ifndef PLE_VERSION_H_INCLUDED
#define PLE_VERSION_H_INCLUDED

#if !defined(PLUGIN_MAJOR_VERSION) || !defined(PLUGIN_MINOR_VERSION) || !defined(PLUGIN_PATCH_VERSION)
#error "Plugin version not defined"
#endif

constexpr ucell PLUGIN_VERSION_KEY = static_cast<ucell>(PLUGIN_MAJOR_VERSION) << 24 |
                                     static_cast<ucell>(PLUGIN_MINOR_VERSION) << 16 |
                                     static_cast<ucell>(PLUGIN_PATCH_VERSION) << 8;

#endif /* PLE_VERSION_H_INCLUDED */