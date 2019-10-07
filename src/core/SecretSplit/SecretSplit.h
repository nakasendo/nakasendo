#ifndef __SECRET_SPLIT_H__
#define __SECRET_SPLIT_H__

#include <SecretSplit/SecretSplitConfig.h>

#include <vector>
#include <SecretSplit/KeyShare.h>

class BigNumber;

SecretSplit_API std::vector<KeyShare> make_shared_secret (const Polynomial& poly, const int& minimum, const int& shares);
SecretSplit_API BigNumber RecoverSecret ( const std::vector<KeyShare>& shares , const BigNumber& mod);

#endif //ifndef __SECRET_SPLIT_H__

