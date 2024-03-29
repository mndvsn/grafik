﻿/**
 * Grafik
 * LabMenu
 * Copyright 2012-2022 Martin Furuberg
 */
#pragma once
#include "labb/Lab.h"


namespace labb
{
    struct LLabMenuItem
    {
        std::string name;
        std::function<LLab*()> createInstance;
    };

    class LLabMenu : public LLab
    {
        std::vector<std::pair<std::string, LLabMenuItem>> _labs;
        bool _bBigMenu { true };

    public:
        LLabMenu() = default;

        void OnUI(UIEvent& e) override;
        
        void BeginLabMenu();
        void BeginRendererMenu();
        void BeginBigMenu();

        template<typename T>
        void RegisterLab(const std::string& name, const std::string& shortName)
        {
            Log::Debug("Adding lab: {0}", name);
            _labs.push_back({ shortName,
                {
                    name,
                    []
                    {
                        return new T { };
                    }
                }
            });
        }

        void CreateLabIfExists(const std::string& labShortName);

        void ShowBigMenu() { _bBigMenu = true; }
        void HideBigMenu() { _bBigMenu = false; }
    };
}
