#include "mini_lsm_app.hpp"

#include <gtest/gtest.h>

#include <cstdlib>

namespace mini_lsm {

TEST(MiniLsmAppTest, Run_NoState_ReturnsSuccess) {
  EXPECT_EQ(run(), EXIT_SUCCESS);
}

}  // namespace mini_lsm
