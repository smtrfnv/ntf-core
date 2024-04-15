// Copyright 2020-2023 Bloomberg Finance L.P.
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <ntccfg_test.h>
#include <bslma_testallocator.h>

using namespace BloombergLP;

//=============================================================================
//                                 TEST PLAN
//-----------------------------------------------------------------------------
//                                 Overview
//                                 --------
//
//-----------------------------------------------------------------------------

// [ 1]
//-----------------------------------------------------------------------------
// [ 1]
//-----------------------------------------------------------------------------

namespace mock_test {

class Interface
{
    virtual void f() = 0;
    virtual int f1() = 0;

    virtual void f2(int) = 0;
    virtual void f3(int*) = 0;
    virtual void f4(int&) = 0;

};

NTF_MOCK_CLASS(MyMock, Interface)

NTF_MOCK_METHOD(void, f)
NTF_MOCK_METHOD(int, f1)

NTF_MOCK_METHOD(void, f2, int)
NTF_MOCK_METHOD(void, f3, int*)
NTF_MOCK_METHOD(void, f4, int&)

NTF_MOCK_CLASS_END;

}

NTCCFG_TEST_CASE(1)
{
    using namespace mock_test;

    MyMock mock;
    NTF_EXPECT_0(mock, f).ONCE();
    mock.f();

    NTF_EXPECT_0(mock, f1).ONCE().RETURN(22);
    NTF_EXPECT_0(mock, f1).ONCE().RETURN(33);

    NTCCFG_TEST_EQ(mock.f1(), 22);
    NTCCFG_TEST_EQ(mock.f1(), 33);
}

NTCCFG_TEST_CASE(2)
{
    using namespace mock_test;

    MyMock mock;

    {
        // it means we do not case what argument is used when f2 is called
        NTF_EXPECT_1(mock, f2, IGNORE_ARG).ONCE();

        const int val = 22;
        mock.f2(val);

        // here we expect that the argument used to call f2 equals `expected`
        const int expected = 22;
        NTF_EXPECT_1(mock, f2, NTF_EQ(expected)).ONCE();
        mock.f2(val);
    }
    {
        int value = 44;
        int *ptr = &value;
        int *expected_ptr = ptr;

        // expect that argument used to call f3 equals `expected_ptr`
        NTF_EXPECT_1(mock, f3, NTF_EQ(expected_ptr)).ONCE();
        mock.f3(ptr);

        //expect that when argument used to call f3 is dereferenced it equals
        //`expected value`
        int expected_value = value;
        NTF_EXPECT_1(mock, f3, NTF_EQ_DEREF(expected_value)).ONCE();
        mock.f3(ptr);
    }
}

NTCCFG_TEST_DRIVER
{
    NTCCFG_TEST_REGISTER(1);
    NTCCFG_TEST_REGISTER(2);
}
NTCCFG_TEST_DRIVER_END;
