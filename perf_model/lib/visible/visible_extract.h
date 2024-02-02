// SPDX-FileCopyrightText: 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_VISIBLE_EXTRACT_H
#define INCLUDE_VISIBLE_EXTRACT_H

#include <vector>

#include "rapidjson/document.h"
#include "visible.h"

void get_staged(std::vector<Staged> &vstaged,
                const rapidjson::Value &stagedArray);

void get_dec(DecodedInstr &decoded, const rapidjson::Value &dec);

void get_pc(PC &retpc, const rapidjson::Value &pc);

void get_next_reentrant(int i, rapidjson::Document &doc);

void parse_visible(rapidjson::Document &doc, std::vector<VisibleState> &items,
                   const std::string &json);

void get_visible(VisibleState &parsedItem, const rapidjson::Value &item);

void get_next_reentrant(int i, VisibleState &parsedItem,
                        rapidjson::Document &doc);

#endif /* end of include guard: INCLUDE_VISIBLE_EXTRACT_H */
