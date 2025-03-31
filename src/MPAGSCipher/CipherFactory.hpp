#ifndef MPAGSCIPHER_CIPHERFACTORY_HPP
#define MPAGSCIPHER_CIPHERFACTORY_HPP

#include <memory>
#include <string>
#include "Cipher.hpp"
#include "CipherType.hpp"

namespace CipherFactory {
  /**
   * \brief Create a cipher of the specified type
   * \param cipherType The type of cipher to create
   * \param key The key to use for the cipher
   * \return A unique pointer to the created cipher
   */
  std::unique_ptr<Cipher> makeCipher(const CipherType type,
                                       const std::string& key);

} // namespace CipherFactory


#endif