#include "Song.h"
#include "Application/Instruments/CommandList.h"
#include "Application/Utils/HexBuffers.h"
#include "System/io/Status.h"
#include "Table.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

Song::Song() : Persistent("SONG") {

  data_ = (unsigned char *)SYS_MALLOC(SONG_CHANNEL_COUNT * SONG_ROW_COUNT);
  memset(data_, 0xFF, SONG_CHANNEL_COUNT * SONG_ROW_COUNT);

  chain_ = new Chain();   // Allocate chain datas
  phrase_ = new Phrase(); // Allocate phrase datas
};

Song::~Song() {
  if (data_ != NULL)
    SYS_FREE(data_);
  delete chain_;
  delete phrase_;
};

void Song::SaveContent(tinyxml2::XMLPrinter *printer) {
  for (int i = 0; i < PHRASE_COUNT * 16; i++) {
    phrase_->param1_[i] = Swap16(phrase_->param1_[i]);
    phrase_->param2_[i] = Swap16(phrase_->param2_[i]);
  }
  saveHexBuffer(printer, "SONG", data_, SONG_ROW_COUNT * SONG_CHANNEL_COUNT);
  saveHexBuffer(printer, "CHAINS", chain_->data_, CHAIN_COUNT * 16);
  saveHexBuffer(printer, "TRANSPOSES", chain_->transpose_, CHAIN_COUNT * 16);
  saveHexBuffer(printer, "NOTES", phrase_->note_, PHRASE_COUNT * 16);
  saveHexBuffer(printer, "INSTRUMENTS", phrase_->instr_, PHRASE_COUNT * 16);
  saveHexBuffer(printer, "COMMAND1", phrase_->cmd1_, PHRASE_COUNT * 16);
  saveHexBuffer(printer, "PARAM1", phrase_->param1_, PHRASE_COUNT * 16);
  saveHexBuffer(printer, "COMMAND2", phrase_->cmd2_, PHRASE_COUNT * 16);
  saveHexBuffer(printer, "PARAM2", phrase_->param2_, PHRASE_COUNT * 16);
};

void Song::RestoreContent(PersistencyDocument *doc) {
  bool elem = doc->FirstChild();

  while (elem) {
    if (!strcmp("SONG", doc->ElemName())) {
      restoreHexBuffer(doc, data_);
    };
    if (!strcmp("CHAINS", doc->ElemName())) {
      restoreHexBuffer(doc, chain_->data_);
    };
    if (!strcmp("TRANSPOSES", doc->ElemName())) {
      restoreHexBuffer(doc, chain_->transpose_);
    };
    if (!strcmp("NOTES", doc->ElemName())) {
      restoreHexBuffer(doc, phrase_->note_);
    };
    if (!strcmp("INSTRUMENTS", doc->ElemName())) {
      restoreHexBuffer(doc, phrase_->instr_);
    };
    if (!strcmp("COMMAND1", doc->ElemName())) {
      restoreHexBuffer(doc, (uchar *)phrase_->cmd1_);
    };
    if (!strcmp("PARAM1", doc->ElemName())) {
      restoreHexBuffer(doc, (uchar *)phrase_->param1_);
      for (int i = 0; i < PHRASE_COUNT * 16; i++) {
        phrase_->param1_[i] = Swap16(phrase_->param1_[i]);
      }
    };
    if (!strcmp("COMMAND2", doc->ElemName())) {
      restoreHexBuffer(doc, (uchar *)phrase_->cmd2_);
    };
    if (!strcmp("PARAM2", doc->ElemName())) {
      restoreHexBuffer(doc, (uchar *)phrase_->param2_);
      for (int i = 0; i < PHRASE_COUNT * 16; i++) {
        phrase_->param2_[i] = Swap16(phrase_->param2_[i]);
      }
    };
    elem = doc->NextSibling();
  }

  Status::Set("Restoring allocation");

  // Restore chain & phrase allocation table

  unsigned char *data = data_;
  for (int i = 0; i < SONG_ROW_COUNT * SONG_CHANNEL_COUNT; i++) {
    if (*data != 0xFF) {
      if (*data < 0x80) {
        chain_->SetUsed(*data);
      }
    }
    data++;
  }

  data = chain_->data_;

  for (int i = 0; i < CHAIN_COUNT; i++) {
    for (int j = 0; j < 16; j++) {
      if (*data != 0xFF) {
        chain_->SetUsed(i);
        phrase_->SetUsed(*data);
      }
      data++;
    };
  }

  data = phrase_->note_;

  FourCC *table1 = phrase_->cmd1_;
  FourCC *table2 = phrase_->cmd2_;

  ushort *param1 = phrase_->param1_;
  ushort *param2 = phrase_->param2_;

  TableHolder *th = TableHolder::GetInstance();

  for (int i = 0; i < PHRASE_COUNT; i++) {
    for (int j = 0; j < 16; j++) {
      if (*data != 0xFF) {
        phrase_->SetUsed(i);
      }
      if (*table1 == I_CMD_TABL) {
        *param1 &= 0x7F;
        th->SetUsed((*param1));
      };
      if (*table2 == I_CMD_TABL) {
        *param2 &= 0x7F;
        th->SetUsed((*param2));
      };
      table1++;
      table2++;
      param1++;
      param2++;
      data++;
    };
  }
};
