// Copyright 2020 Phyronnaz

#pragma warning( disable : 4701 )
#pragma warning( disable : 4244 )

// FastNoise.cpp
//
// MIT License
//
// Copyright(c) 2017 Jordan Peck
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The developer's email is jorzixdan.me2@gzixmail.com (for great email, take
// off every 'zix'.)
//

#include "FastNoise.h"

#include <math.h>
#include <assert.h>

#include <algorithm>
#include <random>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// While random generators are standardized, samplers aren't
// To avoid issues, we use the microsoft std version on all platforms
// License:

/*
 The Microsoft C++ Standard Library is under the Apache License v2.0 with LLVM Exception:

                                 Apache License
                           Version 2.0, January 2004
                        http://www.apache.org/licenses/

   TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION

   1. Definitions.

      "License" shall mean the terms and conditions for use, reproduction,
      and distribution as defined by Sections 1 through 9 of this document.

      "Licensor" shall mean the copyright owner or entity authorized by
      the copyright owner that is granting the License.

      "Legal Entity" shall mean the union of the acting entity and all
      other entities that control, are controlled by, or are under common
      control with that entity. For the purposes of this definition,
      "control" means (i) the power, direct or indirect, to cause the
      direction or management of such entity, whether by contract or
      otherwise, or (ii) ownership of fifty percent (50%) or more of the
      outstanding shares, or (iii) beneficial ownership of such entity.

      "You" (or "Your") shall mean an individual or Legal Entity
      exercising permissions granted by this License.

      "Source" form shall mean the preferred form for making modifications,
      including but not limited to software source code, documentation
      source, and configuration files.

      "Object" form shall mean any form resulting from mechanical
      transformation or translation of a Source form, including but
      not limited to compiled object code, generated documentation,
      and conversions to other media types.

      "Work" shall mean the work of authorship, whether in Source or
      Object form, made available under the License, as indicated by a
      copyright notice that is included in or attached to the work
      (an example is provided in the Appendix below).

      "Derivative Works" shall mean any work, whether in Source or Object
      form, that is based on (or derived from) the Work and for which the
      editorial revisions, annotations, elaborations, or other modifications
      represent, as a whole, an original work of authorship. For the purposes
      of this License, Derivative Works shall not include works that remain
      separable from, or merely link (or bind by name) to the interfaces of,
      the Work and Derivative Works thereof.

      "Contribution" shall mean any work of authorship, including
      the original version of the Work and any modifications or additions
      to that Work or Derivative Works thereof, that is intentionally
      submitted to Licensor for inclusion in the Work by the copyright owner
      or by an individual or Legal Entity authorized to submit on behalf of
      the copyright owner. For the purposes of this definition, "submitted"
      means any form of electronic, verbal, or written communication sent
      to the Licensor or its representatives, including but not limited to
      communication on electronic mailing lists, source code control systems,
      and issue tracking systems that are managed by, or on behalf of, the
      Licensor for the purpose of discussing and improving the Work, but
      excluding communication that is conspicuously marked or otherwise
      designated in writing by the copyright owner as "Not a Contribution."

      "Contributor" shall mean Licensor and any individual or Legal Entity
      on behalf of whom a Contribution has been received by Licensor and
      subsequently incorporated within the Work.

   2. Grant of Copyright License. Subject to the terms and conditions of
      this License, each Contributor hereby grants to You a perpetual,
      worldwide, non-exclusive, no-charge, royalty-free, irrevocable
      copyright license to reproduce, prepare Derivative Works of,
      publicly display, publicly perform, sublicense, and distribute the
      Work and such Derivative Works in Source or Object form.

   3. Grant of Patent License. Subject to the terms and conditions of
      this License, each Contributor hereby grants to You a perpetual,
      worldwide, non-exclusive, no-charge, royalty-free, irrevocable
      (except as stated in this section) patent license to make, have made,
      use, offer to sell, sell, import, and otherwise transfer the Work,
      where such license applies only to those patent claims licensable
      by such Contributor that are necessarily infringed by their
      Contribution(s) alone or by combination of their Contribution(s)
      with the Work to which such Contribution(s) was submitted. If You
      institute patent litigation against any entity (including a
      cross-claim or counterclaim in a lawsuit) alleging that the Work
      or a Contribution incorporated within the Work constitutes direct
      or contributory patent infringement, then any patent licenses
      granted to You under this License for that Work shall terminate
      as of the date such litigation is filed.

   4. Redistribution. You may reproduce and distribute copies of the
      Work or Derivative Works thereof in any medium, with or without
      modifications, and in Source or Object form, provided that You
      meet the following conditions:

      (a) You must give any other recipients of the Work or
          Derivative Works a copy of this License; and

      (b) You must cause any modified files to carry prominent notices
          stating that You changed the files; and

      (c) You must retain, in the Source form of any Derivative Works
          that You distribute, all copyright, patent, trademark, and
          attribution notices from the Source form of the Work,
          excluding those notices that do not pertain to any part of
          the Derivative Works; and

      (d) If the Work includes a "NOTICE" text file as part of its
          distribution, then any Derivative Works that You distribute must
          include a readable copy of the attribution notices contained
          within such NOTICE file, excluding those notices that do not
          pertain to any part of the Derivative Works, in at least one
          of the following places: within a NOTICE text file distributed
          as part of the Derivative Works; within the Source form or
          documentation, if provided along with the Derivative Works; or,
          within a display generated by the Derivative Works, if and
          wherever such third-party notices normally appear. The contents
          of the NOTICE file are for informational purposes only and
          do not modify the License. You may add Your own attribution
          notices within Derivative Works that You distribute, alongside
          or as an addendum to the NOTICE text from the Work, provided
          that such additional attribution notices cannot be construed
          as modifying the License.

      You may add Your own copyright statement to Your modifications and
      may provide additional or different license terms and conditions
      for use, reproduction, or distribution of Your modifications, or
      for any such Derivative Works as a whole, provided Your use,
      reproduction, and distribution of the Work otherwise complies with
      the conditions stated in this License.

   5. Submission of Contributions. Unless You explicitly state otherwise,
      any Contribution intentionally submitted for inclusion in the Work
      by You to the Licensor shall be under the terms and conditions of
      this License, without any additional terms or conditions.
      Notwithstanding the above, nothing herein shall supersede or modify
      the terms of any separate license agreement you may have executed
      with Licensor regarding such Contributions.

   6. Trademarks. This License does not grant permission to use the trade
      names, trademarks, service marks, or product names of the Licensor,
      except as required for reasonable and customary use in describing the
      origin of the Work and reproducing the content of the NOTICE file.

   7. Disclaimer of Warranty. Unless required by applicable law or
      agreed to in writing, Licensor provides the Work (and each
      Contributor provides its Contributions) on an "AS IS" BASIS,
      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
      implied, including, without limitation, any warranties or conditions
      of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A
      PARTICULAR PURPOSE. You are solely responsible for determining the
      appropriateness of using or redistributing the Work and assume any
      risks associated with Your exercise of permissions under this License.

   8. Limitation of Liability. In no event and under no legal theory,
      whether in tort (including negligence), contract, or otherwise,
      unless required by applicable law (such as deliberate and grossly
      negligent acts) or agreed to in writing, shall any Contributor be
      liable to You for damages, including any direct, indirect, special,
      incidental, or consequential damages of any character arising as a
      result of this License or out of the use or inability to use the
      Work (including but not limited to damages for loss of goodwill,
      work stoppage, computer failure or malfunction, or any and all
      other commercial damages or losses), even if such Contributor
      has been advised of the possibility of such damages.

   9. Accepting Warranty or Additional Liability. While redistributing
      the Work or Derivative Works thereof, You may choose to offer,
      and charge a fee for, acceptance of support, warranty, indemnity,
      or other liability obligations and/or rights consistent with this
      License. However, in accepting such obligations, You may act only
      on Your own behalf and on Your sole responsibility, not on behalf
      of any other Contributor, and only if You agree to indemnify,
      defend, and hold each Contributor harmless for any liability
      incurred by, or claims asserted against, such Contributor by reason
      of your accepting any such warranty or additional liability.

   END OF TERMS AND CONDITIONS

   APPENDIX: How to apply the Apache License to your work.

      To apply the Apache License to your work, attach the following
      boilerplate notice, with the fields enclosed by brackets "[]"
      replaced with your own identifying information. (Don't include
      the brackets!)  The text should be enclosed in the appropriate
      comment syntax for the file format. We also recommend that a
      file or class name and description of purpose be included on the
      same "printed page" as the copyright notice for easier
      identification within third-party archives.

   Copyright [yyyy] [name of copyright owner]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

---- LLVM Exceptions to the Apache 2.0 License ----

As an exception, if, as a result of your compiling your source code, portions
of this Software are embedded into an Object form of such source code, you
may redistribute such embedded portions in such Object form without complying
with the conditions of Sections 4(a), 4(b) and 4(d) of the License.

In addition, if you combine or link compiled forms of this Software with
software that is licensed under the GPLv2 ("Combined Software") and if a
court of competent jurisdiction determines that the patent provision (Section
3), the indemnity provision (Section 9) or other Section of the License
conflicts with the conditions of the GPLv2, you may retroactively and
prospectively choose to deem waived or otherwise exclude such Section(s) of
the License, but only in their entirety and only with respect to the Combined
Software.
*/

namespace cross_platform_std
{
	// CLASS TEMPLATE _Rng_from_urng
	template <class _Diff, class _Urng>
	class _Rng_from_urng { // wrap a URNG as an RNG
	public:
		using _Ty0 = std::make_unsigned_t<_Diff>;
		using _Ty1 = typename _Urng::result_type;

		using _Udiff = std::conditional_t < sizeof(_Ty1) < sizeof(_Ty0), _Ty0, _Ty1 > ;

		explicit _Rng_from_urng(_Urng& _Func) : _Ref(_Func), _Bits(8 * sizeof(_Udiff)), _Bmask(_Udiff(-1)) {
			for (; (_Urng::max)() - (_Urng::min)() < _Bmask; _Bmask >>= 1) {
				--_Bits;
			}
		}

		_Diff operator()(_Diff _Index) { // adapt _Urng closed range to [0, _Index)
			for (;;) { // try a sample random value
				_Udiff _Ret = 0; // random bits
				_Udiff _Mask = 0; // 2^N - 1, _Ret is within [0, _Mask]

				while (_Mask < _Udiff(_Index - 1)) { // need more random bits
					_Ret <<= _Bits - 1; // avoid full shift
					_Ret <<= 1;
					_Ret |= _Get_bits();
					_Mask <<= _Bits - 1; // avoid full shift
					_Mask <<= 1;
					_Mask |= _Bmask;
				}

				// _Ret is [0, _Mask], _Index - 1 <= _Mask, return if unbiased
				if (_Ret / _Index < _Mask / _Index || _Mask % _Index == _Udiff(_Index - 1)) {
					return static_cast<_Diff>(_Ret % _Index);
				}
			}
		}

		_Udiff _Get_all_bits() { // return a random value
			_Udiff _Ret = 0;

			for (size_t _Num = 0; _Num < 8 * sizeof(_Udiff); _Num += _Bits) { // don't mask away any bits
				_Ret <<= _Bits - 1; // avoid full shift
				_Ret <<= 1;
				_Ret |= _Get_bits();
			}

			return _Ret;
		}

		_Rng_from_urng(const _Rng_from_urng&) = delete;
		_Rng_from_urng& operator=(const _Rng_from_urng&) = delete;

	private:
		_Udiff _Get_bits() { // return a random value within [0, _Bmask]
			for (;;) { // repeat until random value is in range
				_Udiff _Val = _Ref() - (_Urng::min)();

				if (_Val <= _Bmask) {
					return _Val;
				}
			}
		}

		_Urng& _Ref; // reference to URNG
		size_t _Bits; // number of random bits generated by _Get_bits()
		_Udiff _Bmask; // 2^_Bits - 1
	};

	// CLASS TEMPLATE uniform_int
	template <class _Ty = int>
	class uniform_int { // uniform integer distribution
	public:
		using result_type = _Ty;

		struct param_type { // parameter package
			using distribution_type = uniform_int;

			explicit param_type(result_type _Min0 = 0, result_type _Max0 = 9) { // construct from parameters
				_Init(_Min0, _Max0);
			}

			bool operator==(const param_type& _Right) const { // test for equality
				return _Min == _Right._Min && _Max == _Right._Max;
			}

			bool operator!=(const param_type& _Right) const { // test for inequality
				return !(*this == _Right);
			}

			result_type a() const { // return a value
				return _Min;
			}

			result_type b() const { // return b value
				return _Max;
			}

			void _Init(_Ty _Min0, _Ty _Max0) { // set internal state
				_Min = _Min0;
				_Max = _Max0;
			}

			result_type _Min;
			result_type _Max;
		};

		explicit uniform_int(_Ty _Min0 = 0, _Ty _Max0 = 9) : _Par(_Min0, _Max0) { // construct from parameters
		}

		explicit uniform_int(const param_type& _Par0) : _Par(_Par0) { // construct from parameter package
		}

		result_type a() const { // return a value
			return _Par.a();
		}

		result_type b() const { // return b value
			return _Par.b();
		}

		param_type param() const { // return parameter package
			return _Par;
		}

		void param(const param_type& _Par0) { // set parameter package
			_Par = _Par0;
		}

		result_type(min)() const { // return minimum possible generated value
			return _Par._Min;
		}

		result_type(max)() const { // return maximum possible generated value
			return _Par._Max;
		}

		void reset() { // clear internal state
		}

		template <class _Engine>
		result_type operator()(_Engine& _Eng) const { // return next value
			return _Eval(_Eng, _Par._Min, _Par._Max);
		}

		template <class _Engine>
		result_type operator()(
			_Engine& _Eng, const param_type& _Par0) const { // return next value, given parameter package
			return _Eval(_Eng, _Par0._Min, _Par0._Max);
		}

		template <class _Engine>
		result_type operator()(_Engine& _Eng, result_type _Nx) const { // return next value
			return _Eval(_Eng, 0, _Nx - 1);
		}

		template <class _Elem, class _Traits>
		std::basic_istream<_Elem, _Traits>& _Read(std::basic_istream<_Elem, _Traits>& _Istr) { // read state from _Istr
			_Ty _Min0;
			_Ty _Max0;
			_Istr >> _Min0 >> _Max0;
			_Par._Init(_Min0, _Max0);
			return _Istr;
		}

		template <class _Elem, class _Traits>
		std::basic_ostream<_Elem, _Traits>& _Write(std::basic_ostream<_Elem, _Traits>& _Ostr) const { // write state to _Ostr
			return _Ostr << _Par._Min << ' ' << _Par._Max;
		}

	private:
		using _Uty = std::make_unsigned_t<_Ty>;

		template <class _Engine>
		result_type _Eval(_Engine& _Eng, _Ty _Min, _Ty _Max) const { // compute next value in range [_Min, _Max]
			_Rng_from_urng<_Uty, _Engine> _Rng(_Eng);

			const _Uty _Umin = _Adjust(_Uty(_Min));
			const _Uty _Umax = _Adjust(_Uty(_Max));

			_Uty _Uret;

			if (_Umax - _Umin == _Uty(-1)) {
				_Uret = static_cast<_Uty>(_Rng._Get_all_bits());
			}
			else {
				_Uret = static_cast<_Uty>(_Rng(static_cast<_Uty>(_Umax - _Umin + 1)));
			}

			return _Ty(_Adjust(static_cast<_Uty>(_Uret + _Umin)));
		}

		static _Uty _Adjust(_Uty _Uval) { // convert signed ranges to unsigned ranges and vice versa
			return _Adjust(_Uval, std::is_signed<_Ty>());
		}

		static _Uty _Adjust(_Uty _Uval, std::true_type) { // convert signed ranges to unsigned ranges and vice versa
			const _Uty _Adjuster = (_Uty(-1) >> 1) + 1; // 2^(N-1)

			if (_Uval < _Adjuster) {
				return static_cast<_Uty>(_Uval + _Adjuster);
			}
			else {
				return static_cast<_Uty>(_Uval - _Adjuster);
			}
		}

		static _Uty _Adjust(_Uty _Uval, std::false_type) { // _Ty is already unsigned, do nothing
			return _Uval;
		}

		param_type _Par;
	};

	template <class _Elem, class _Traits, class _Ty>
	std::basic_istream<_Elem, _Traits>& operator>>(std::basic_istream<_Elem, _Traits>& _Istr,
		uniform_int<_Ty>& _Dist) { // read state from _Istr
		return _Dist._Read(_Istr);
	}

	template <class _Elem, class _Traits, class _Ty>
	std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& _Ostr,
		const uniform_int<_Ty>& _Dist) { // write state to _Ostr
		return _Dist._Write(_Ostr);
	}

	// CLASS TEMPLATE uniform_int_distribution
	template <class _Ty = int>
	class uniform_int_distribution : public uniform_int<_Ty> { // uniform integer distribution
	public:
		using _Mybase     = uniform_int<_Ty>;
		using _Mypbase    = typename _Mybase::param_type;
		using result_type = typename _Mybase::result_type;

		struct param_type : public _Mypbase { // parameter package
			using distribution_type = uniform_int_distribution;

			explicit param_type(result_type _Min0 = 0, result_type _Max0 = (std::numeric_limits<_Ty>::max)())
				: _Mypbase(_Min0, _Max0) { // construct from parameters
			}

			param_type(const _Mypbase& _Right) : _Mypbase(_Right) { // construct from base
			}
		};

		explicit uniform_int_distribution(_Ty _Min0 = 0, _Ty _Max0 = (std::numeric_limits<_Ty>::max)())
			: _Mybase(_Min0, _Max0) { // construct from parameters
		}

		explicit uniform_int_distribution(const param_type& _Par0) : _Mybase(_Par0) { // construct from parameter package
		}
	};

	template <class _Ty>
	bool operator==(const uniform_int_distribution<_Ty>& _Left,
		const uniform_int_distribution<_Ty>& _Right) { // test for equality
		return _Left.param() == _Right.param();
	}

	template <class _Ty>
	bool operator!=(const uniform_int_distribution<_Ty>& _Left,
		const uniform_int_distribution<_Ty>& _Right) { // test for inequality
		return !(_Left == _Right);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

const FN_DECIMAL GRAD_X[] =
{
	1, -1, 1, -1,
	1, -1, 1, -1,
	0, 0, 0, 0
};
const FN_DECIMAL GRAD_Y[] =
{
	1, 1, -1, -1,
	0, 0, 0, 0,
	1, -1, 1, -1
};
const FN_DECIMAL GRAD_Z[] =
{
	0, 0, 0, 0,
	1, 1, -1, -1,
	1, 1, -1, -1
};

const FN_DECIMAL GRAD_4D[] =
{
	0,1,1,1,0,1,1,-1,0,1,-1,1,0,1,-1,-1,
	0,-1,1,1,0,-1,1,-1,0,-1,-1,1,0,-1,-1,-1,
	1,0,1,1,1,0,1,-1,1,0,-1,1,1,0,-1,-1,
	-1,0,1,1,-1,0,1,-1,-1,0,-1,1,-1,0,-1,-1,
	1,1,0,1,1,1,0,-1,1,-1,0,1,1,-1,0,-1,
	-1,1,0,1,-1,1,0,-1,-1,-1,0,1,-1,-1,0,-1,
	1,1,1,0,1,1,-1,0,1,-1,1,0,1,-1,-1,0,
	-1,1,1,0,-1,1,-1,0,-1,-1,1,0,-1,-1,-1,0
};

const FN_DECIMAL VAL_LUT[] =
{
	FN_DECIMAL(0.3490196078), FN_DECIMAL(0.4352941176), FN_DECIMAL(-0.4509803922), FN_DECIMAL(0.6392156863), FN_DECIMAL(0.5843137255), FN_DECIMAL(-0.1215686275), FN_DECIMAL(0.7176470588), FN_DECIMAL(-0.1058823529), FN_DECIMAL(0.3960784314), FN_DECIMAL(0.0431372549), FN_DECIMAL(-0.03529411765), FN_DECIMAL(0.3176470588), FN_DECIMAL(0.7254901961), FN_DECIMAL(0.137254902), FN_DECIMAL(0.8588235294), FN_DECIMAL(-0.8196078431),
	FN_DECIMAL(-0.7960784314), FN_DECIMAL(-0.3333333333), FN_DECIMAL(-0.6705882353), FN_DECIMAL(-0.3882352941), FN_DECIMAL(0.262745098), FN_DECIMAL(0.3254901961), FN_DECIMAL(-0.6470588235), FN_DECIMAL(-0.9215686275), FN_DECIMAL(-0.5294117647), FN_DECIMAL(0.5294117647), FN_DECIMAL(-0.4666666667), FN_DECIMAL(0.8117647059), FN_DECIMAL(0.3803921569), FN_DECIMAL(0.662745098), FN_DECIMAL(0.03529411765), FN_DECIMAL(-0.6156862745),
	FN_DECIMAL(-0.01960784314), FN_DECIMAL(-0.3568627451), FN_DECIMAL(-0.09019607843), FN_DECIMAL(0.7490196078), FN_DECIMAL(0.8352941176), FN_DECIMAL(-0.4039215686), FN_DECIMAL(-0.7490196078), FN_DECIMAL(0.9529411765), FN_DECIMAL(-0.0431372549), FN_DECIMAL(-0.9294117647), FN_DECIMAL(-0.6549019608), FN_DECIMAL(0.9215686275), FN_DECIMAL(-0.06666666667), FN_DECIMAL(-0.4431372549), FN_DECIMAL(0.4117647059), FN_DECIMAL(-0.4196078431),
	FN_DECIMAL(-0.7176470588), FN_DECIMAL(-0.8117647059), FN_DECIMAL(-0.2549019608), FN_DECIMAL(0.4901960784), FN_DECIMAL(0.9137254902), FN_DECIMAL(0.7882352941), FN_DECIMAL(-1.0), FN_DECIMAL(-0.4745098039), FN_DECIMAL(0.7960784314), FN_DECIMAL(0.8509803922), FN_DECIMAL(-0.6784313725), FN_DECIMAL(0.4588235294), FN_DECIMAL(1.0), FN_DECIMAL(-0.1843137255), FN_DECIMAL(0.4509803922), FN_DECIMAL(0.1450980392),
	FN_DECIMAL(-0.231372549), FN_DECIMAL(-0.968627451), FN_DECIMAL(-0.8588235294), FN_DECIMAL(0.4274509804), FN_DECIMAL(0.003921568627), FN_DECIMAL(-0.003921568627), FN_DECIMAL(0.2156862745), FN_DECIMAL(0.5058823529), FN_DECIMAL(0.7647058824), FN_DECIMAL(0.2078431373), FN_DECIMAL(-0.5921568627), FN_DECIMAL(0.5764705882), FN_DECIMAL(-0.1921568627), FN_DECIMAL(-0.937254902), FN_DECIMAL(0.08235294118), FN_DECIMAL(-0.08235294118),
	FN_DECIMAL(0.9058823529), FN_DECIMAL(0.8274509804), FN_DECIMAL(0.02745098039), FN_DECIMAL(-0.168627451), FN_DECIMAL(-0.7803921569), FN_DECIMAL(0.1137254902), FN_DECIMAL(-0.9450980392), FN_DECIMAL(0.2), FN_DECIMAL(0.01960784314), FN_DECIMAL(0.5607843137), FN_DECIMAL(0.2705882353), FN_DECIMAL(0.4431372549), FN_DECIMAL(-0.9607843137), FN_DECIMAL(0.6156862745), FN_DECIMAL(0.9294117647), FN_DECIMAL(-0.07450980392),
	FN_DECIMAL(0.3098039216), FN_DECIMAL(0.9921568627), FN_DECIMAL(-0.9137254902), FN_DECIMAL(-0.2941176471), FN_DECIMAL(-0.3411764706), FN_DECIMAL(-0.6235294118), FN_DECIMAL(-0.7647058824), FN_DECIMAL(-0.8901960784), FN_DECIMAL(0.05882352941), FN_DECIMAL(0.2392156863), FN_DECIMAL(0.7333333333), FN_DECIMAL(0.6549019608), FN_DECIMAL(0.2470588235), FN_DECIMAL(0.231372549), FN_DECIMAL(-0.3960784314), FN_DECIMAL(-0.05098039216),
	FN_DECIMAL(-0.2235294118), FN_DECIMAL(-0.3725490196), FN_DECIMAL(0.6235294118), FN_DECIMAL(0.7019607843), FN_DECIMAL(-0.8274509804), FN_DECIMAL(0.4196078431), FN_DECIMAL(0.07450980392), FN_DECIMAL(0.8666666667), FN_DECIMAL(-0.537254902), FN_DECIMAL(-0.5058823529), FN_DECIMAL(-0.8039215686), FN_DECIMAL(0.09019607843), FN_DECIMAL(-0.4823529412), FN_DECIMAL(0.6705882353), FN_DECIMAL(-0.7882352941), FN_DECIMAL(0.09803921569),
	FN_DECIMAL(-0.6078431373), FN_DECIMAL(0.8039215686), FN_DECIMAL(-0.6), FN_DECIMAL(-0.3254901961), FN_DECIMAL(-0.4117647059), FN_DECIMAL(-0.01176470588), FN_DECIMAL(0.4823529412), FN_DECIMAL(0.168627451), FN_DECIMAL(0.8745098039), FN_DECIMAL(-0.3647058824), FN_DECIMAL(-0.1607843137), FN_DECIMAL(0.568627451), FN_DECIMAL(-0.9921568627), FN_DECIMAL(0.9450980392), FN_DECIMAL(0.5137254902), FN_DECIMAL(0.01176470588),
	FN_DECIMAL(-0.1450980392), FN_DECIMAL(-0.5529411765), FN_DECIMAL(-0.5764705882), FN_DECIMAL(-0.1137254902), FN_DECIMAL(0.5215686275), FN_DECIMAL(0.1607843137), FN_DECIMAL(0.3725490196), FN_DECIMAL(-0.2), FN_DECIMAL(-0.7254901961), FN_DECIMAL(0.631372549), FN_DECIMAL(0.7098039216), FN_DECIMAL(-0.568627451), FN_DECIMAL(0.1294117647), FN_DECIMAL(-0.3098039216), FN_DECIMAL(0.7411764706), FN_DECIMAL(-0.8509803922),
	FN_DECIMAL(0.2549019608), FN_DECIMAL(-0.6392156863), FN_DECIMAL(-0.5607843137), FN_DECIMAL(-0.3176470588), FN_DECIMAL(0.937254902), FN_DECIMAL(0.9843137255), FN_DECIMAL(0.5921568627), FN_DECIMAL(0.6941176471), FN_DECIMAL(0.2862745098), FN_DECIMAL(-0.5215686275), FN_DECIMAL(0.1764705882), FN_DECIMAL(0.537254902), FN_DECIMAL(-0.4901960784), FN_DECIMAL(-0.4588235294), FN_DECIMAL(-0.2078431373), FN_DECIMAL(-0.2156862745),
	FN_DECIMAL(0.7725490196), FN_DECIMAL(0.3647058824), FN_DECIMAL(-0.2392156863), FN_DECIMAL(0.2784313725), FN_DECIMAL(-0.8823529412), FN_DECIMAL(0.8980392157), FN_DECIMAL(0.1215686275), FN_DECIMAL(0.1058823529), FN_DECIMAL(-0.8745098039), FN_DECIMAL(-0.9843137255), FN_DECIMAL(-0.7019607843), FN_DECIMAL(0.9607843137), FN_DECIMAL(0.2941176471), FN_DECIMAL(0.3411764706), FN_DECIMAL(0.1529411765), FN_DECIMAL(0.06666666667),
	FN_DECIMAL(-0.9764705882), FN_DECIMAL(0.3019607843), FN_DECIMAL(0.6470588235), FN_DECIMAL(-0.5843137255), FN_DECIMAL(0.05098039216), FN_DECIMAL(-0.5137254902), FN_DECIMAL(-0.137254902), FN_DECIMAL(0.3882352941), FN_DECIMAL(-0.262745098), FN_DECIMAL(-0.3019607843), FN_DECIMAL(-0.1764705882), FN_DECIMAL(-0.7568627451), FN_DECIMAL(0.1843137255), FN_DECIMAL(-0.5450980392), FN_DECIMAL(-0.4980392157), FN_DECIMAL(-0.2784313725),
	FN_DECIMAL(-0.9529411765), FN_DECIMAL(-0.09803921569), FN_DECIMAL(0.8901960784), FN_DECIMAL(-0.2862745098), FN_DECIMAL(-0.3803921569), FN_DECIMAL(0.5529411765), FN_DECIMAL(0.7803921569), FN_DECIMAL(-0.8352941176), FN_DECIMAL(0.6862745098), FN_DECIMAL(0.7568627451), FN_DECIMAL(0.4980392157), FN_DECIMAL(-0.6862745098), FN_DECIMAL(-0.8980392157), FN_DECIMAL(-0.7725490196), FN_DECIMAL(-0.7098039216), FN_DECIMAL(-0.2470588235),
	FN_DECIMAL(-0.9058823529), FN_DECIMAL(0.9764705882), FN_DECIMAL(0.1921568627), FN_DECIMAL(0.8431372549), FN_DECIMAL(-0.05882352941), FN_DECIMAL(0.3568627451), FN_DECIMAL(0.6078431373), FN_DECIMAL(0.5450980392), FN_DECIMAL(0.4039215686), FN_DECIMAL(-0.7333333333), FN_DECIMAL(-0.4274509804), FN_DECIMAL(0.6), FN_DECIMAL(0.6784313725), FN_DECIMAL(-0.631372549), FN_DECIMAL(-0.02745098039), FN_DECIMAL(-0.1294117647),
	FN_DECIMAL(0.3333333333), FN_DECIMAL(-0.8431372549), FN_DECIMAL(0.2235294118), FN_DECIMAL(-0.3490196078), FN_DECIMAL(-0.6941176471), FN_DECIMAL(0.8823529412), FN_DECIMAL(0.4745098039), FN_DECIMAL(0.4666666667), FN_DECIMAL(-0.7411764706), FN_DECIMAL(-0.2705882353), FN_DECIMAL(0.968627451), FN_DECIMAL(0.8196078431), FN_DECIMAL(-0.662745098), FN_DECIMAL(-0.4352941176), FN_DECIMAL(-0.8666666667), FN_DECIMAL(-0.1529411765),
};

const FN_DECIMAL CELL_2D_X[] =
{
	FN_DECIMAL(-0.6440658039), FN_DECIMAL(-0.08028078721), FN_DECIMAL(0.9983546168), FN_DECIMAL(0.9869492062), FN_DECIMAL(0.9284746418), FN_DECIMAL(0.6051097552), FN_DECIMAL(-0.794167404), FN_DECIMAL(-0.3488667991), FN_DECIMAL(-0.943136526), FN_DECIMAL(-0.9968171318), FN_DECIMAL(0.8740961579), FN_DECIMAL(0.1421139764), FN_DECIMAL(0.4282553608), FN_DECIMAL(-0.9986665833), FN_DECIMAL(0.9996760121), FN_DECIMAL(-0.06248383632),
	FN_DECIMAL(0.7120139305), FN_DECIMAL(0.8917660409), FN_DECIMAL(0.1094842955), FN_DECIMAL(-0.8730880804), FN_DECIMAL(0.2594811489), FN_DECIMAL(-0.6690063346), FN_DECIMAL(-0.9996834972), FN_DECIMAL(-0.8803608671), FN_DECIMAL(-0.8166554937), FN_DECIMAL(0.8955599676), FN_DECIMAL(-0.9398321388), FN_DECIMAL(0.07615451399), FN_DECIMAL(-0.7147270565), FN_DECIMAL(0.8707354457), FN_DECIMAL(-0.9580008579), FN_DECIMAL(0.4905965632),
	FN_DECIMAL(0.786775944), FN_DECIMAL(0.1079711577), FN_DECIMAL(0.2686638979), FN_DECIMAL(0.6113487322), FN_DECIMAL(-0.530770584), FN_DECIMAL(-0.7837268286), FN_DECIMAL(-0.8558691039), FN_DECIMAL(-0.5726093896), FN_DECIMAL(-0.9830740914), FN_DECIMAL(0.7087766359), FN_DECIMAL(0.6807027153), FN_DECIMAL(-0.08864708788), FN_DECIMAL(0.6704485923), FN_DECIMAL(-0.1350735482), FN_DECIMAL(-0.9381333003), FN_DECIMAL(0.9756655376),
	FN_DECIMAL(0.4231433671), FN_DECIMAL(-0.4959787385), FN_DECIMAL(0.1005554325), FN_DECIMAL(-0.7645857281), FN_DECIMAL(-0.5859053796), FN_DECIMAL(-0.9751154306), FN_DECIMAL(-0.6972258572), FN_DECIMAL(0.7907012002), FN_DECIMAL(-0.9109899213), FN_DECIMAL(-0.9584307894), FN_DECIMAL(-0.8269529333), FN_DECIMAL(0.2608264719), FN_DECIMAL(-0.7773760119), FN_DECIMAL(0.7606456974), FN_DECIMAL(-0.8961083758), FN_DECIMAL(-0.9838134719),
	FN_DECIMAL(0.7338893576), FN_DECIMAL(0.2161226729), FN_DECIMAL(0.673509891), FN_DECIMAL(-0.5512056873), FN_DECIMAL(0.6899744332), FN_DECIMAL(0.868004831), FN_DECIMAL(0.5897430311), FN_DECIMAL(-0.8950444221), FN_DECIMAL(-0.3595752773), FN_DECIMAL(0.8209486981), FN_DECIMAL(-0.2912360132), FN_DECIMAL(-0.9965011374), FN_DECIMAL(0.9766994634), FN_DECIMAL(0.738790822), FN_DECIMAL(-0.4730947722), FN_DECIMAL(0.8946479441),
	FN_DECIMAL(-0.6943628971), FN_DECIMAL(-0.6620468182), FN_DECIMAL(-0.0887255502), FN_DECIMAL(-0.7512250855), FN_DECIMAL(-0.5322986898), FN_DECIMAL(0.5226295385), FN_DECIMAL(0.2296318375), FN_DECIMAL(0.7915307344), FN_DECIMAL(-0.2756485999), FN_DECIMAL(-0.6900234522), FN_DECIMAL(0.07090588086), FN_DECIMAL(0.5981278485), FN_DECIMAL(0.3033429312), FN_DECIMAL(-0.7253142797), FN_DECIMAL(-0.9855874307), FN_DECIMAL(-0.1761843396),
	FN_DECIMAL(-0.6438468325), FN_DECIMAL(-0.9956136595), FN_DECIMAL(0.8541580762), FN_DECIMAL(-0.9999807666), FN_DECIMAL(-0.02152416253), FN_DECIMAL(-0.8705983095), FN_DECIMAL(-0.1197138014), FN_DECIMAL(-0.992107781), FN_DECIMAL(-0.9091181546), FN_DECIMAL(0.788610536), FN_DECIMAL(-0.994636402), FN_DECIMAL(0.4211256853), FN_DECIMAL(0.3110430857), FN_DECIMAL(-0.4031127839), FN_DECIMAL(0.7610684239), FN_DECIMAL(0.7685674467),
	FN_DECIMAL(0.152271555), FN_DECIMAL(-0.9364648723), FN_DECIMAL(0.1681333739), FN_DECIMAL(-0.3567427907), FN_DECIMAL(-0.418445483), FN_DECIMAL(-0.98774778), FN_DECIMAL(0.8705250765), FN_DECIMAL(-0.8911701067), FN_DECIMAL(-0.7315350966), FN_DECIMAL(0.6030885658), FN_DECIMAL(-0.4149130821), FN_DECIMAL(0.7585339481), FN_DECIMAL(0.6963196535), FN_DECIMAL(0.8332685012), FN_DECIMAL(-0.8086815232), FN_DECIMAL(0.7518116724),
	FN_DECIMAL(-0.3490535894), FN_DECIMAL(0.6972110903), FN_DECIMAL(-0.8795676928), FN_DECIMAL(-0.6442331882), FN_DECIMAL(0.6610236811), FN_DECIMAL(-0.9853565782), FN_DECIMAL(-0.590338458), FN_DECIMAL(0.09843602117), FN_DECIMAL(0.5646534882), FN_DECIMAL(-0.6023259233), FN_DECIMAL(-0.3539248861), FN_DECIMAL(0.5132728656), FN_DECIMAL(0.9380385118), FN_DECIMAL(-0.7599270056), FN_DECIMAL(-0.7425936564), FN_DECIMAL(-0.6679610562),
	FN_DECIMAL(-0.3018497816), FN_DECIMAL(0.814478266), FN_DECIMAL(0.03777430269), FN_DECIMAL(-0.7514235086), FN_DECIMAL(0.9662556939), FN_DECIMAL(-0.4720194901), FN_DECIMAL(-0.435054126), FN_DECIMAL(0.7091901235), FN_DECIMAL(0.929379209), FN_DECIMAL(0.9997434357), FN_DECIMAL(0.8306320299), FN_DECIMAL(-0.9434019629), FN_DECIMAL(-0.133133759), FN_DECIMAL(0.5048413216), FN_DECIMAL(0.3711995273), FN_DECIMAL(0.98552091),
	FN_DECIMAL(0.7401857005), FN_DECIMAL(-0.9999981398), FN_DECIMAL(-0.2144033253), FN_DECIMAL(0.4808624681), FN_DECIMAL(-0.413835885), FN_DECIMAL(0.644229305), FN_DECIMAL(0.9626648696), FN_DECIMAL(0.1833665934), FN_DECIMAL(0.5794129), FN_DECIMAL(0.01404446873), FN_DECIMAL(0.4388494993), FN_DECIMAL(0.5213612322), FN_DECIMAL(-0.5281609948), FN_DECIMAL(-0.9745306846), FN_DECIMAL(-0.9904373013), FN_DECIMAL(0.9100232252),
	FN_DECIMAL(-0.9914057719), FN_DECIMAL(0.7892627765), FN_DECIMAL(0.3364421659), FN_DECIMAL(-0.9416099764), FN_DECIMAL(0.7802732656), FN_DECIMAL(0.886302871), FN_DECIMAL(0.6524471291), FN_DECIMAL(0.5762186726), FN_DECIMAL(-0.08987644664), FN_DECIMAL(-0.2177026782), FN_DECIMAL(-0.9720345052), FN_DECIMAL(-0.05722538858), FN_DECIMAL(0.8105983127), FN_DECIMAL(0.3410261032), FN_DECIMAL(0.6452309645), FN_DECIMAL(-0.7810612152),
	FN_DECIMAL(0.9989395718), FN_DECIMAL(-0.808247815), FN_DECIMAL(0.6370177929), FN_DECIMAL(0.5844658772), FN_DECIMAL(0.2054070861), FN_DECIMAL(0.055960522), FN_DECIMAL(-0.995827561), FN_DECIMAL(0.893409165), FN_DECIMAL(-0.931516824), FN_DECIMAL(0.328969469), FN_DECIMAL(-0.3193837488), FN_DECIMAL(0.7314755657), FN_DECIMAL(-0.7913517714), FN_DECIMAL(-0.2204109786), FN_DECIMAL(0.9955900414), FN_DECIMAL(-0.7112353139),
	FN_DECIMAL(-0.7935008741), FN_DECIMAL(-0.9961918204), FN_DECIMAL(-0.9714163995), FN_DECIMAL(-0.9566188669), FN_DECIMAL(0.2748495632), FN_DECIMAL(-0.4681743221), FN_DECIMAL(-0.9614449642), FN_DECIMAL(0.585194072), FN_DECIMAL(0.4532946061), FN_DECIMAL(-0.9916113176), FN_DECIMAL(0.942479587), FN_DECIMAL(-0.9813704753), FN_DECIMAL(-0.6538429571), FN_DECIMAL(0.2923335053), FN_DECIMAL(-0.2246660704), FN_DECIMAL(-0.1800781949),
	FN_DECIMAL(-0.9581216256), FN_DECIMAL(0.552215082), FN_DECIMAL(-0.9296791922), FN_DECIMAL(0.643183699), FN_DECIMAL(0.9997325981), FN_DECIMAL(-0.4606920354), FN_DECIMAL(-0.2148721265), FN_DECIMAL(0.3482070809), FN_DECIMAL(0.3075517813), FN_DECIMAL(0.6274756393), FN_DECIMAL(0.8910881765), FN_DECIMAL(-0.6397771309), FN_DECIMAL(-0.4479080125), FN_DECIMAL(-0.5247665011), FN_DECIMAL(-0.8386507094), FN_DECIMAL(0.3901291416),
	FN_DECIMAL(0.1458336921), FN_DECIMAL(0.01624613149), FN_DECIMAL(-0.8273199879), FN_DECIMAL(0.5611100679), FN_DECIMAL(-0.8380219841), FN_DECIMAL(-0.9856122234), FN_DECIMAL(-0.861398618), FN_DECIMAL(0.6398413916), FN_DECIMAL(0.2694510795), FN_DECIMAL(0.4327334514), FN_DECIMAL(-0.9960265354), FN_DECIMAL(-0.939570655), FN_DECIMAL(-0.8846996446), FN_DECIMAL(0.7642113189), FN_DECIMAL(-0.7002080528), FN_DECIMAL(0.664508256),
};
const FN_DECIMAL CELL_2D_Y[] =
{
	FN_DECIMAL(0.7649700911), FN_DECIMAL(0.9967722885), FN_DECIMAL(0.05734160033), FN_DECIMAL(-0.1610318741), FN_DECIMAL(0.371395799), FN_DECIMAL(-0.7961420628), FN_DECIMAL(0.6076990492), FN_DECIMAL(-0.9371723195), FN_DECIMAL(0.3324056156), FN_DECIMAL(0.07972205329), FN_DECIMAL(-0.4857529277), FN_DECIMAL(-0.9898503007), FN_DECIMAL(0.9036577593), FN_DECIMAL(0.05162417479), FN_DECIMAL(-0.02545330525), FN_DECIMAL(-0.998045976),
	FN_DECIMAL(-0.7021653386), FN_DECIMAL(-0.4524967717), FN_DECIMAL(-0.9939885256), FN_DECIMAL(-0.4875625128), FN_DECIMAL(-0.9657481729), FN_DECIMAL(-0.7432567015), FN_DECIMAL(0.02515761212), FN_DECIMAL(0.4743044842), FN_DECIMAL(0.5771254669), FN_DECIMAL(0.4449408324), FN_DECIMAL(0.3416365773), FN_DECIMAL(0.9970960285), FN_DECIMAL(0.6994034849), FN_DECIMAL(0.4917517499), FN_DECIMAL(0.286765333), FN_DECIMAL(0.8713868327),
	FN_DECIMAL(0.6172387009), FN_DECIMAL(0.9941540269), FN_DECIMAL(0.9632339851), FN_DECIMAL(-0.7913613129), FN_DECIMAL(0.847515538), FN_DECIMAL(0.6211056739), FN_DECIMAL(0.5171924952), FN_DECIMAL(-0.8198283277), FN_DECIMAL(-0.1832084353), FN_DECIMAL(0.7054329737), FN_DECIMAL(0.7325597678), FN_DECIMAL(0.9960630973), FN_DECIMAL(0.7419559859), FN_DECIMAL(0.9908355749), FN_DECIMAL(-0.346274329), FN_DECIMAL(0.2192641299),
	FN_DECIMAL(-0.9060627411), FN_DECIMAL(-0.8683346653), FN_DECIMAL(0.9949314574), FN_DECIMAL(-0.6445220433), FN_DECIMAL(-0.8103794704), FN_DECIMAL(-0.2216977607), FN_DECIMAL(0.7168515217), FN_DECIMAL(0.612202264), FN_DECIMAL(-0.412428616), FN_DECIMAL(0.285325116), FN_DECIMAL(0.56227115), FN_DECIMAL(-0.9653857009), FN_DECIMAL(-0.6290361962), FN_DECIMAL(0.6491672535), FN_DECIMAL(0.443835306), FN_DECIMAL(-0.1791955706),
	FN_DECIMAL(-0.6792690269), FN_DECIMAL(-0.9763662173), FN_DECIMAL(0.7391782104), FN_DECIMAL(0.8343693968), FN_DECIMAL(0.7238337389), FN_DECIMAL(0.4965557504), FN_DECIMAL(0.8075909592), FN_DECIMAL(-0.4459769977), FN_DECIMAL(-0.9331160806), FN_DECIMAL(-0.5710019572), FN_DECIMAL(0.9566512346), FN_DECIMAL(-0.08357920318), FN_DECIMAL(0.2146116448), FN_DECIMAL(-0.6739348049), FN_DECIMAL(0.8810115417), FN_DECIMAL(0.4467718167),
	FN_DECIMAL(-0.7196250184), FN_DECIMAL(-0.749462481), FN_DECIMAL(0.9960561112), FN_DECIMAL(0.6600461127), FN_DECIMAL(-0.8465566164), FN_DECIMAL(-0.8525598897), FN_DECIMAL(-0.9732775654), FN_DECIMAL(0.6111293616), FN_DECIMAL(-0.9612584717), FN_DECIMAL(-0.7237870097), FN_DECIMAL(-0.9974830104), FN_DECIMAL(-0.8014006968), FN_DECIMAL(0.9528814544), FN_DECIMAL(-0.6884178931), FN_DECIMAL(-0.1691668301), FN_DECIMAL(0.9843571905),
	FN_DECIMAL(0.7651544003), FN_DECIMAL(-0.09355982605), FN_DECIMAL(-0.5200134429), FN_DECIMAL(-0.006202125807), FN_DECIMAL(-0.9997683284), FN_DECIMAL(0.4919944954), FN_DECIMAL(-0.9928084436), FN_DECIMAL(-0.1253880012), FN_DECIMAL(-0.4165383308), FN_DECIMAL(-0.6148930171), FN_DECIMAL(-0.1034332049), FN_DECIMAL(-0.9070022917), FN_DECIMAL(-0.9503958117), FN_DECIMAL(0.9151503065), FN_DECIMAL(-0.6486716073), FN_DECIMAL(0.6397687707),
	FN_DECIMAL(-0.9883386937), FN_DECIMAL(0.3507613761), FN_DECIMAL(0.9857642561), FN_DECIMAL(-0.9342026446), FN_DECIMAL(-0.9082419159), FN_DECIMAL(0.1560587169), FN_DECIMAL(0.4921240607), FN_DECIMAL(-0.453669308), FN_DECIMAL(0.6818037859), FN_DECIMAL(0.7976742329), FN_DECIMAL(0.9098610522), FN_DECIMAL(0.651633524), FN_DECIMAL(0.7177318024), FN_DECIMAL(-0.5528685241), FN_DECIMAL(0.5882467118), FN_DECIMAL(0.6593778956),
	FN_DECIMAL(0.9371027648), FN_DECIMAL(-0.7168658839), FN_DECIMAL(-0.4757737632), FN_DECIMAL(0.7648291307), FN_DECIMAL(0.7503650398), FN_DECIMAL(0.1705063456), FN_DECIMAL(-0.8071558121), FN_DECIMAL(-0.9951433815), FN_DECIMAL(-0.8253280792), FN_DECIMAL(-0.7982502628), FN_DECIMAL(0.9352738503), FN_DECIMAL(0.8582254747), FN_DECIMAL(-0.3465310238), FN_DECIMAL(0.65000842), FN_DECIMAL(-0.6697422351), FN_DECIMAL(0.7441962291),
	FN_DECIMAL(-0.9533555), FN_DECIMAL(0.5801940659), FN_DECIMAL(-0.9992862963), FN_DECIMAL(-0.659820211), FN_DECIMAL(0.2575848092), FN_DECIMAL(0.881588113), FN_DECIMAL(-0.9004043022), FN_DECIMAL(-0.7050172826), FN_DECIMAL(0.369126382), FN_DECIMAL(-0.02265088836), FN_DECIMAL(0.5568217228), FN_DECIMAL(-0.3316515286), FN_DECIMAL(0.991098079), FN_DECIMAL(-0.863212164), FN_DECIMAL(-0.9285531277), FN_DECIMAL(0.1695539323),
	FN_DECIMAL(-0.672402505), FN_DECIMAL(-0.001928841934), FN_DECIMAL(0.9767452145), FN_DECIMAL(-0.8767960349), FN_DECIMAL(0.9103515037), FN_DECIMAL(-0.7648324016), FN_DECIMAL(0.2706960452), FN_DECIMAL(-0.9830446035), FN_DECIMAL(0.8150341657), FN_DECIMAL(-0.9999013716), FN_DECIMAL(-0.8985605806), FN_DECIMAL(0.8533360801), FN_DECIMAL(0.8491442537), FN_DECIMAL(-0.2242541966), FN_DECIMAL(-0.1379635899), FN_DECIMAL(-0.4145572694),
	FN_DECIMAL(0.1308227633), FN_DECIMAL(0.6140555916), FN_DECIMAL(0.9417041303), FN_DECIMAL(-0.336705587), FN_DECIMAL(-0.6254387508), FN_DECIMAL(0.4631060578), FN_DECIMAL(-0.7578342456), FN_DECIMAL(-0.8172955655), FN_DECIMAL(-0.9959529228), FN_DECIMAL(-0.9760151351), FN_DECIMAL(0.2348380732), FN_DECIMAL(-0.9983612848), FN_DECIMAL(0.5856025746), FN_DECIMAL(-0.9400538266), FN_DECIMAL(-0.7639875669), FN_DECIMAL(0.6244544645),
	FN_DECIMAL(0.04604054566), FN_DECIMAL(0.5888424828), FN_DECIMAL(0.7708490978), FN_DECIMAL(-0.8114182882), FN_DECIMAL(0.9786766212), FN_DECIMAL(-0.9984329822), FN_DECIMAL(0.09125496582), FN_DECIMAL(-0.4492438803), FN_DECIMAL(-0.3636982357), FN_DECIMAL(0.9443405575), FN_DECIMAL(-0.9476254645), FN_DECIMAL(-0.6818676535), FN_DECIMAL(-0.6113610831), FN_DECIMAL(0.9754070948), FN_DECIMAL(-0.0938108173), FN_DECIMAL(-0.7029540015),
	FN_DECIMAL(-0.6085691109), FN_DECIMAL(-0.08718862881), FN_DECIMAL(-0.237381926), FN_DECIMAL(0.2913423132), FN_DECIMAL(0.9614872426), FN_DECIMAL(0.8836361266), FN_DECIMAL(-0.2749974196), FN_DECIMAL(-0.8108932717), FN_DECIMAL(-0.8913607575), FN_DECIMAL(0.129255541), FN_DECIMAL(-0.3342637104), FN_DECIMAL(-0.1921249337), FN_DECIMAL(-0.7566302845), FN_DECIMAL(-0.9563164339), FN_DECIMAL(-0.9744358146), FN_DECIMAL(0.9836522982),
	FN_DECIMAL(-0.2863615732), FN_DECIMAL(0.8337016872), FN_DECIMAL(0.3683701937), FN_DECIMAL(0.7657119102), FN_DECIMAL(-0.02312427772), FN_DECIMAL(0.8875600535), FN_DECIMAL(0.976642191), FN_DECIMAL(0.9374176384), FN_DECIMAL(0.9515313457), FN_DECIMAL(-0.7786361937), FN_DECIMAL(-0.4538302125), FN_DECIMAL(-0.7685604874), FN_DECIMAL(-0.8940796454), FN_DECIMAL(-0.8512462154), FN_DECIMAL(0.5446696133), FN_DECIMAL(0.9207601495),
	FN_DECIMAL(-0.9893091197), FN_DECIMAL(-0.9998680229), FN_DECIMAL(0.5617309299), FN_DECIMAL(-0.8277411985), FN_DECIMAL(0.545636467), FN_DECIMAL(0.1690223212), FN_DECIMAL(-0.5079295433), FN_DECIMAL(0.7685069899), FN_DECIMAL(-0.9630140787), FN_DECIMAL(0.9015219132), FN_DECIMAL(0.08905695279), FN_DECIMAL(-0.3423550559), FN_DECIMAL(-0.4661614943), FN_DECIMAL(-0.6449659371), FN_DECIMAL(0.7139388509), FN_DECIMAL(0.7472809229),
};
const FN_DECIMAL CELL_3D_X[] =
{
	FN_DECIMAL(0.3752498686), FN_DECIMAL(0.687188096), FN_DECIMAL(0.2248135212), FN_DECIMAL(0.6692006647), FN_DECIMAL(-0.4376476931), FN_DECIMAL(0.6139972552), FN_DECIMAL(0.9494563929), FN_DECIMAL(0.8065108882), FN_DECIMAL(-0.2218812853), FN_DECIMAL(0.8484661167), FN_DECIMAL(0.5551817596), FN_DECIMAL(0.2133903499), FN_DECIMAL(0.5195126593), FN_DECIMAL(-0.6440141975), FN_DECIMAL(-0.5192897331), FN_DECIMAL(-0.3697654077),
	FN_DECIMAL(-0.07927779647), FN_DECIMAL(0.4187757321), FN_DECIMAL(-0.750078731), FN_DECIMAL(0.6579554632), FN_DECIMAL(-0.6859803838), FN_DECIMAL(-0.6878407087), FN_DECIMAL(0.9490848347), FN_DECIMAL(0.5795829433), FN_DECIMAL(-0.5325976529), FN_DECIMAL(-0.1363699466), FN_DECIMAL(0.417665879), FN_DECIMAL(-0.9108236468), FN_DECIMAL(0.4438605427), FN_DECIMAL(0.819294887), FN_DECIMAL(-0.4033873915), FN_DECIMAL(-0.2817317705),
	FN_DECIMAL(0.3969665622), FN_DECIMAL(0.5323450134), FN_DECIMAL(-0.6833017297), FN_DECIMAL(0.3881436661), FN_DECIMAL(-0.7119144767), FN_DECIMAL(-0.2306979838), FN_DECIMAL(-0.9398873022), FN_DECIMAL(0.1701906676), FN_DECIMAL(-0.4261839496), FN_DECIMAL(-0.003712295499), FN_DECIMAL(-0.734675004), FN_DECIMAL(-0.3195046015), FN_DECIMAL(0.7345307424), FN_DECIMAL(0.9766246496), FN_DECIMAL(-0.02003735175), FN_DECIMAL(-0.4824156342),
	FN_DECIMAL(0.4245892007), FN_DECIMAL(0.9072427669), FN_DECIMAL(0.593346808), FN_DECIMAL(-0.8911762541), FN_DECIMAL(-0.7657571834), FN_DECIMAL(-0.5268198896), FN_DECIMAL(-0.8801903279), FN_DECIMAL(-0.6296409617), FN_DECIMAL(-0.09492481344), FN_DECIMAL(-0.4920470525), FN_DECIMAL(0.7307666154), FN_DECIMAL(-0.2514540636), FN_DECIMAL(-0.3356210347), FN_DECIMAL(-0.3522787894), FN_DECIMAL(0.87847885), FN_DECIMAL(-0.7424096346),
	FN_DECIMAL(0.5757585274), FN_DECIMAL(0.4519299338), FN_DECIMAL(0.6420368628), FN_DECIMAL(-0.1128478447), FN_DECIMAL(0.499874883), FN_DECIMAL(0.5291681739), FN_DECIMAL(-0.5098837195), FN_DECIMAL(0.5639583502), FN_DECIMAL(-0.8456386526), FN_DECIMAL(-0.9657134875), FN_DECIMAL(-0.576437342), FN_DECIMAL(-0.5666013014), FN_DECIMAL(0.5667702405), FN_DECIMAL(-0.481316582), FN_DECIMAL(0.7313389916), FN_DECIMAL(-0.3805628566),
	FN_DECIMAL(-0.6512675909), FN_DECIMAL(-0.2787156951), FN_DECIMAL(0.8648059114), FN_DECIMAL(-0.9730216276), FN_DECIMAL(-0.8335820906), FN_DECIMAL(0.2673159641), FN_DECIMAL(0.231150148), FN_DECIMAL(0.01286214638), FN_DECIMAL(0.6774953261), FN_DECIMAL(0.6542885718), FN_DECIMAL(-0.02545450161), FN_DECIMAL(0.2101238586), FN_DECIMAL(-0.5572105885), FN_DECIMAL(0.813705672), FN_DECIMAL(-0.7546026951), FN_DECIMAL(-0.2502500006),
	FN_DECIMAL(-0.9979289381), FN_DECIMAL(0.7024037039), FN_DECIMAL(0.08990874624), FN_DECIMAL(0.8170812432), FN_DECIMAL(0.4226980265), FN_DECIMAL(-0.2442153475), FN_DECIMAL(-0.9183326731), FN_DECIMAL(0.6068222411), FN_DECIMAL(0.818676691), FN_DECIMAL(-0.7236735282), FN_DECIMAL(-0.5383903295), FN_DECIMAL(-0.6269337242), FN_DECIMAL(-0.0939331121), FN_DECIMAL(0.9203878539), FN_DECIMAL(-0.7256396824), FN_DECIMAL(0.6292431149),
	FN_DECIMAL(0.4234156978), FN_DECIMAL(0.006685688024), FN_DECIMAL(-0.2598694113), FN_DECIMAL(0.6408036421), FN_DECIMAL(0.05899871622), FN_DECIMAL(0.7090281418), FN_DECIMAL(-0.5905222072), FN_DECIMAL(0.3128214264), FN_DECIMAL(-0.691925826), FN_DECIMAL(0.3634019349), FN_DECIMAL(-0.6772511147), FN_DECIMAL(-0.3204583896), FN_DECIMAL(-0.3906740409), FN_DECIMAL(-0.3342190395), FN_DECIMAL(-0.517779592), FN_DECIMAL(-0.6817711267),
	FN_DECIMAL(0.6422383105), FN_DECIMAL(0.4388482478), FN_DECIMAL(0.2968562611), FN_DECIMAL(-0.2019778353), FN_DECIMAL(0.6014865048), FN_DECIMAL(0.9519280722), FN_DECIMAL(0.3398889569), FN_DECIMAL(0.8179709354), FN_DECIMAL(0.2365522154), FN_DECIMAL(0.3262175096), FN_DECIMAL(-0.8060715954), FN_DECIMAL(-0.2068642503), FN_DECIMAL(0.6208057279), FN_DECIMAL(-0.5274282502), FN_DECIMAL(-0.3722334928), FN_DECIMAL(-0.8923412971),
	FN_DECIMAL(0.5341834201), FN_DECIMAL(-0.3663701513), FN_DECIMAL(-0.6114600319), FN_DECIMAL(0.5026307556), FN_DECIMAL(0.8396151729), FN_DECIMAL(0.9245042467), FN_DECIMAL(-0.7994843957), FN_DECIMAL(-0.5357200589), FN_DECIMAL(-0.6283359739), FN_DECIMAL(-0.61351886), FN_DECIMAL(-0.875632008), FN_DECIMAL(-0.5278879423), FN_DECIMAL(0.9087491985), FN_DECIMAL(-0.03500215466), FN_DECIMAL(-0.261365798), FN_DECIMAL(-0.579523541),
	FN_DECIMAL(-0.3765052689), FN_DECIMAL(-0.74398252), FN_DECIMAL(0.4257318052), FN_DECIMAL(-0.1214508921), FN_DECIMAL(0.8561809753), FN_DECIMAL(0.6802835104), FN_DECIMAL(-0.5452131039), FN_DECIMAL(-0.1997156478), FN_DECIMAL(0.4562348357), FN_DECIMAL(-0.811704301), FN_DECIMAL(0.67793962), FN_DECIMAL(-0.9237819106), FN_DECIMAL(0.6973511259), FN_DECIMAL(-0.5189506), FN_DECIMAL(0.5517320032), FN_DECIMAL(-0.396710831),
	FN_DECIMAL(0.5493762815), FN_DECIMAL(-0.2507853002), FN_DECIMAL(0.4788634005), FN_DECIMAL(0.387333516), FN_DECIMAL(-0.2176515694), FN_DECIMAL(0.6749832419), FN_DECIMAL(0.2148283022), FN_DECIMAL(-0.7521815872), FN_DECIMAL(0.4697000159), FN_DECIMAL(0.7890593699), FN_DECIMAL(-0.7606162952), FN_DECIMAL(0.01083397843), FN_DECIMAL(0.5254091908), FN_DECIMAL(-0.6748025877), FN_DECIMAL(0.751091524), FN_DECIMAL(0.05259056135),
	FN_DECIMAL(0.01889481232), FN_DECIMAL(-0.6037423727), FN_DECIMAL(-0.6542965129), FN_DECIMAL(0.08873301081), FN_DECIMAL(-0.6191345671), FN_DECIMAL(0.4331858488), FN_DECIMAL(-0.3858351946), FN_DECIMAL(-0.1429059747), FN_DECIMAL(0.4118221036), FN_DECIMAL(-0.6247153214), FN_DECIMAL(-0.611423014), FN_DECIMAL(0.5542939606), FN_DECIMAL(-0.9432768808), FN_DECIMAL(-0.4567870451), FN_DECIMAL(-0.7349133547), FN_DECIMAL(0.399304489),
	FN_DECIMAL(-0.7474927672), FN_DECIMAL(0.02589419753), FN_DECIMAL(0.783915821), FN_DECIMAL(0.6138668752), FN_DECIMAL(0.4276376047), FN_DECIMAL(-0.4347886353), FN_DECIMAL(0.02947841302), FN_DECIMAL(-0.833742746), FN_DECIMAL(0.3817221742), FN_DECIMAL(-0.8743368359), FN_DECIMAL(-0.3823443796), FN_DECIMAL(-0.6829243811), FN_DECIMAL(-0.3681903049), FN_DECIMAL(-0.367626833), FN_DECIMAL(-0.434583373), FN_DECIMAL(0.235891995),
	FN_DECIMAL(-0.6874880269), FN_DECIMAL(-0.5115661773), FN_DECIMAL(-0.5534962601), FN_DECIMAL(0.5632777056), FN_DECIMAL(0.686191532), FN_DECIMAL(-0.05095871588), FN_DECIMAL(-0.06865785057), FN_DECIMAL(-0.5975288531), FN_DECIMAL(-0.6429790056), FN_DECIMAL(-0.3729361548), FN_DECIMAL(0.2237917666), FN_DECIMAL(0.6046773225), FN_DECIMAL(-0.5041542295), FN_DECIMAL(-0.03972191174), FN_DECIMAL(0.7028828406), FN_DECIMAL(-0.5560856498),
	FN_DECIMAL(0.5898328456), FN_DECIMAL(-0.9308076766), FN_DECIMAL(0.4617069864), FN_DECIMAL(0.3190983137), FN_DECIMAL(0.9116567753), FN_DECIMAL(-0.45029554), FN_DECIMAL(0.3346334459), FN_DECIMAL(0.8525005645), FN_DECIMAL(0.2528483381), FN_DECIMAL(-0.8306630147), FN_DECIMAL(-0.6880390622), FN_DECIMAL(0.7448684026), FN_DECIMAL(-0.1963355843), FN_DECIMAL(-0.5900257974), FN_DECIMAL(0.9097057294), FN_DECIMAL(-0.2509196808),
};
const FN_DECIMAL CELL_3D_Y[] =
{
	FN_DECIMAL(-0.6760585049), FN_DECIMAL(-0.09136176499), FN_DECIMAL(0.1681325679), FN_DECIMAL(-0.6688468686), FN_DECIMAL(-0.4822753902), FN_DECIMAL(-0.7891068824), FN_DECIMAL(-0.1877509944), FN_DECIMAL(0.548470914), FN_DECIMAL(-0.463339443), FN_DECIMAL(-0.4050542082), FN_DECIMAL(0.3218158513), FN_DECIMAL(0.2546493823), FN_DECIMAL(-0.3753271935), FN_DECIMAL(0.4745384887), FN_DECIMAL(0.481254652), FN_DECIMAL(-0.8934416489),
	FN_DECIMAL(-0.6737085076), FN_DECIMAL(0.7469917228), FN_DECIMAL(0.3826230411), FN_DECIMAL(0.6751013678), FN_DECIMAL(-0.7248119515), FN_DECIMAL(-0.3224276742), FN_DECIMAL(-0.02076190936), FN_DECIMAL(-0.6404268166), FN_DECIMAL(-0.5292028444), FN_DECIMAL(0.7151414636), FN_DECIMAL(-0.6144655059), FN_DECIMAL(-0.369912124), FN_DECIMAL(0.6942067212), FN_DECIMAL(-0.4481558248), FN_DECIMAL(-0.6366894559), FN_DECIMAL(0.5956568471),
	FN_DECIMAL(0.564274539), FN_DECIMAL(0.7145584688), FN_DECIMAL(0.6871918316), FN_DECIMAL(0.5657918509), FN_DECIMAL(-0.6275978114), FN_DECIMAL(0.4146983062), FN_DECIMAL(0.2638993789), FN_DECIMAL(-0.792633138), FN_DECIMAL(0.5706133514), FN_DECIMAL(0.8606546462), FN_DECIMAL(0.6490900316), FN_DECIMAL(-0.8242699196), FN_DECIMAL(0.6765819124), FN_DECIMAL(0.1959534069), FN_DECIMAL(-0.8426769757), FN_DECIMAL(-0.5917672797),
	FN_DECIMAL(0.7517364266), FN_DECIMAL(0.03252559226), FN_DECIMAL(0.0883617105), FN_DECIMAL(0.4475064813), FN_DECIMAL(-0.1418643552), FN_DECIMAL(0.7343428473), FN_DECIMAL(0.3870192548), FN_DECIMAL(-0.7716703522), FN_DECIMAL(0.4839898327), FN_DECIMAL(0.7437439055), FN_DECIMAL(-0.5989573348), FN_DECIMAL(-0.8357068955), FN_DECIMAL(0.6086049038), FN_DECIMAL(0.9194627258), FN_DECIMAL(0.4718297238), FN_DECIMAL(-0.2650335884),
	FN_DECIMAL(-0.6470352599), FN_DECIMAL(-0.5555181303), FN_DECIMAL(0.1222351235), FN_DECIMAL(0.7802044684), FN_DECIMAL(-0.8636947022), FN_DECIMAL(-0.2341352163), FN_DECIMAL(0.683030874), FN_DECIMAL(-0.5005858287), FN_DECIMAL(0.2334616211), FN_DECIMAL(0.2576877608), FN_DECIMAL(0.6666816727), FN_DECIMAL(-0.7663996863), FN_DECIMAL(0.794201982), FN_DECIMAL(0.6189308788), FN_DECIMAL(0.6071033261), FN_DECIMAL(-0.4206058253),
	FN_DECIMAL(-0.3957336915), FN_DECIMAL(-0.8170257484), FN_DECIMAL(-0.1043240417), FN_DECIMAL(0.0002167596213), FN_DECIMAL(0.1816339018), FN_DECIMAL(-0.6838094939), FN_DECIMAL(-0.2495341969), FN_DECIMAL(-0.7116756954), FN_DECIMAL(-0.03361673621), FN_DECIMAL(-0.3350836431), FN_DECIMAL(0.2137186039), FN_DECIMAL(0.2557996786), FN_DECIMAL(0.7490117093), FN_DECIMAL(0.4942936549), FN_DECIMAL(-0.352686853), FN_DECIMAL(-0.3952445435),
	FN_DECIMAL(-0.0459964767), FN_DECIMAL(-0.7115787471), FN_DECIMAL(0.08022899756), FN_DECIMAL(0.5362268157), FN_DECIMAL(-0.8258613686), FN_DECIMAL(0.1114171723), FN_DECIMAL(0.3882823051), FN_DECIMAL(-0.7915404457), FN_DECIMAL(0.3250957662), FN_DECIMAL(0.6401346464), FN_DECIMAL(-0.2662724517), FN_DECIMAL(-0.6727907114), FN_DECIMAL(-0.994730818), FN_DECIMAL(-0.3596358977), FN_DECIMAL(0.2344610069), FN_DECIMAL(-0.6645215546),
	FN_DECIMAL(-0.7107590611), FN_DECIMAL(-0.4646617327), FN_DECIMAL(0.6717191355), FN_DECIMAL(0.5101893498), FN_DECIMAL(0.1185768238), FN_DECIMAL(0.236005093), FN_DECIMAL(-0.7811024061), FN_DECIMAL(0.5089325193), FN_DECIMAL(0.6073187658), FN_DECIMAL(-0.7930732557), FN_DECIMAL(-0.6822767155), FN_DECIMAL(0.3201532885), FN_DECIMAL(0.7545302807), FN_DECIMAL(0.1072664448), FN_DECIMAL(0.6784033173), FN_DECIMAL(-0.6595924967),
	FN_DECIMAL(0.7276509498), FN_DECIMAL(0.5586689436), FN_DECIMAL(-0.6498636788), FN_DECIMAL(0.6789333174), FN_DECIMAL(0.7105966551), FN_DECIMAL(-0.2872214155), FN_DECIMAL(0.496746217), FN_DECIMAL(-0.3880337977), FN_DECIMAL(0.7324070604), FN_DECIMAL(-0.9326634749), FN_DECIMAL(-0.5867839255), FN_DECIMAL(0.8003043651), FN_DECIMAL(-0.1631882481), FN_DECIMAL(-0.6796374681), FN_DECIMAL(-0.8066678503), FN_DECIMAL(0.4238177418),
	FN_DECIMAL(0.7715863549), FN_DECIMAL(0.5455367347), FN_DECIMAL(-0.03205115397), FN_DECIMAL(-0.6005545066), FN_DECIMAL(-0.5423640002), FN_DECIMAL(0.3569205906), FN_DECIMAL(-0.582071752), FN_DECIMAL(0.6407354361), FN_DECIMAL(0.7777142984), FN_DECIMAL(-0.09956428618), FN_DECIMAL(0.1100002681), FN_DECIMAL(0.8136349123), FN_DECIMAL(0.2923431904), FN_DECIMAL(0.9735794425), FN_DECIMAL(0.8324974864), FN_DECIMAL(-0.6179617717),
	FN_DECIMAL(-0.9248386523), FN_DECIMAL(-0.6448780771), FN_DECIMAL(-0.5274402761), FN_DECIMAL(-0.7862170565), FN_DECIMAL(0.2682099744), FN_DECIMAL(-0.5848777694), FN_DECIMAL(-0.6364561467), FN_DECIMAL(-0.7167402514), FN_DECIMAL(-0.8677012494), FN_DECIMAL(0.4205286707), FN_DECIMAL(-0.7007832749), FN_DECIMAL(0.243272451), FN_DECIMAL(-0.1899846085), FN_DECIMAL(-0.6146124977), FN_DECIMAL(-0.8093357692), FN_DECIMAL(-0.03545096987),
	FN_DECIMAL(-0.7191590868), FN_DECIMAL(0.7478645848), FN_DECIMAL(0.3623517328), FN_DECIMAL(0.8436992512), FN_DECIMAL(-0.2445711729), FN_DECIMAL(0.6897356637), FN_DECIMAL(-0.1708070787), FN_DECIMAL(0.4639272368), FN_DECIMAL(-0.7917186656), FN_DECIMAL(0.02980025428), FN_DECIMAL(0.6334156172), FN_DECIMAL(-0.9815544807), FN_DECIMAL(-0.2307217304), FN_DECIMAL(0.1080823318), FN_DECIMAL(0.5167601798), FN_DECIMAL(-0.845120016),
	FN_DECIMAL(0.441572562), FN_DECIMAL(0.5876789172), FN_DECIMAL(-0.6365908737), FN_DECIMAL(0.68350166), FN_DECIMAL(0.5849723959), FN_DECIMAL(0.1164114357), FN_DECIMAL(-0.7379813884), FN_DECIMAL(-0.9613237178), FN_DECIMAL(-0.9071943084), FN_DECIMAL(-0.7682111105), FN_DECIMAL(0.639074459), FN_DECIMAL(-0.619358298), FN_DECIMAL(0.2807257131), FN_DECIMAL(-0.01800868791), FN_DECIMAL(0.3776607289), FN_DECIMAL(0.7207567823),
	FN_DECIMAL(0.5536661486), FN_DECIMAL(-0.9974053117), FN_DECIMAL(-0.02047200006), FN_DECIMAL(-0.6739453804), FN_DECIMAL(-0.5607471297), FN_DECIMAL(0.8815553192), FN_DECIMAL(0.8275977415), FN_DECIMAL(0.3928902456), FN_DECIMAL(0.550991396), FN_DECIMAL(0.4247623676), FN_DECIMAL(-0.3436948871), FN_DECIMAL(-0.3653537677), FN_DECIMAL(0.3181702902), FN_DECIMAL(-0.6067173171), FN_DECIMAL(-0.8984128477), FN_DECIMAL(0.4220839766),
	FN_DECIMAL(0.7238407199), FN_DECIMAL(-0.7766913695), FN_DECIMAL(0.6460037842), FN_DECIMAL(0.2544775664), FN_DECIMAL(0.6488840578), FN_DECIMAL(0.805016833), FN_DECIMAL(-0.9183807036), FN_DECIMAL(0.4144046357), FN_DECIMAL(0.270587208), FN_DECIMAL(-0.8813684494), FN_DECIMAL(0.6985971877), FN_DECIMAL(-0.7795603017), FN_DECIMAL(-0.8624480731), FN_DECIMAL(0.5532697017), FN_DECIMAL(0.711179521), FN_DECIMAL(-0.7798160574),
	FN_DECIMAL(0.5225859041), FN_DECIMAL(0.1261859368), FN_DECIMAL(0.3398033582), FN_DECIMAL(-0.7472173667), FN_DECIMAL(-0.4032647119), FN_DECIMAL(-0.4246578154), FN_DECIMAL(0.8481212377), FN_DECIMAL(-0.2144838537), FN_DECIMAL(0.3431714491), FN_DECIMAL(0.5310188231), FN_DECIMAL(0.6682978632), FN_DECIMAL(0.3110433206), FN_DECIMAL(0.9263293599), FN_DECIMAL(-0.6155600569), FN_DECIMAL(0.07169784399), FN_DECIMAL(0.8985888773),
};
const FN_DECIMAL CELL_3D_Z[] =
{
	FN_DECIMAL(-0.6341391283), FN_DECIMAL(-0.7207118346), FN_DECIMAL(0.9597866014), FN_DECIMAL(0.3237504235), FN_DECIMAL(-0.7588642466), FN_DECIMAL(-0.01782410481), FN_DECIMAL(0.2515593809), FN_DECIMAL(0.2207257205), FN_DECIMAL(-0.8579541106), FN_DECIMAL(0.3406410681), FN_DECIMAL(0.7669470462), FN_DECIMAL(-0.9431957648), FN_DECIMAL(0.7676171537), FN_DECIMAL(-0.6000491115), FN_DECIMAL(-0.7062096948), FN_DECIMAL(0.2550207115),
	FN_DECIMAL(0.7347325213), FN_DECIMAL(0.5163625202), FN_DECIMAL(-0.5394270162), FN_DECIMAL(0.3336656285), FN_DECIMAL(-0.0638635111), FN_DECIMAL(-0.6503195787), FN_DECIMAL(0.3143356798), FN_DECIMAL(-0.5039217245), FN_DECIMAL(0.6605180464), FN_DECIMAL(-0.6855479011), FN_DECIMAL(-0.6693185756), FN_DECIMAL(0.1832083647), FN_DECIMAL(-0.5666258437), FN_DECIMAL(0.3576482138), FN_DECIMAL(-0.6571949095), FN_DECIMAL(-0.7522101635),
	FN_DECIMAL(-0.7238865886), FN_DECIMAL(0.4538887323), FN_DECIMAL(0.2467106257), FN_DECIMAL(0.7274778869), FN_DECIMAL(0.3151170655), FN_DECIMAL(-0.8802293764), FN_DECIMAL(-0.2167232729), FN_DECIMAL(0.5854637865), FN_DECIMAL(0.7019741052), FN_DECIMAL(0.5091756071), FN_DECIMAL(0.1973189533), FN_DECIMAL(0.46743546), FN_DECIMAL(0.05197599597), FN_DECIMAL(0.088354718), FN_DECIMAL(0.5380464843), FN_DECIMAL(-0.6458224544),
	FN_DECIMAL(-0.5045952393), FN_DECIMAL(0.419347884), FN_DECIMAL(0.8000823542), FN_DECIMAL(-0.07445020656), FN_DECIMAL(-0.6272881641), FN_DECIMAL(-0.428020311), FN_DECIMAL(-0.2747382083), FN_DECIMAL(-0.08987283726), FN_DECIMAL(0.8699098354), FN_DECIMAL(0.4524761885), FN_DECIMAL(-0.3274603257), FN_DECIMAL(0.4882262167), FN_DECIMAL(-0.7189983256), FN_DECIMAL(0.1746079907), FN_DECIMAL(0.0751772698), FN_DECIMAL(-0.6152927202),
	FN_DECIMAL(0.4998474673), FN_DECIMAL(-0.6979677227), FN_DECIMAL(0.7568667263), FN_DECIMAL(-0.6152612058), FN_DECIMAL(0.06447140991), FN_DECIMAL(-0.8155744872), FN_DECIMAL(-0.5229602449), FN_DECIMAL(0.6567836838), FN_DECIMAL(-0.4799905631), FN_DECIMAL(0.03153534591), FN_DECIMAL(0.4724992466), FN_DECIMAL(-0.3026458097), FN_DECIMAL(-0.2191225827), FN_DECIMAL(-0.620692287), FN_DECIMAL(0.3107552588), FN_DECIMAL(0.8235670294),
	FN_DECIMAL(0.6474915988), FN_DECIMAL(-0.5047637941), FN_DECIMAL(0.4911488878), FN_DECIMAL(-0.2307138167), FN_DECIMAL(-0.5216800015), FN_DECIMAL(0.6789305939), FN_DECIMAL(0.9403734863), FN_DECIMAL(0.702390397), FN_DECIMAL(0.7347584625), FN_DECIMAL(0.6779567958), FN_DECIMAL(0.9765635805), FN_DECIMAL(-0.9436177661), FN_DECIMAL(-0.358465925), FN_DECIMAL(-0.3058706624), FN_DECIMAL(0.5533414464), FN_DECIMAL(-0.8838306897),
	FN_DECIMAL(0.04496841812), FN_DECIMAL(0.01687374963), FN_DECIMAL(-0.9927133148), FN_DECIMAL(-0.211752318), FN_DECIMAL(0.3732015249), FN_DECIMAL(0.9632990593), FN_DECIMAL(-0.07682417004), FN_DECIMAL(-0.07232213047), FN_DECIMAL(0.4733721775), FN_DECIMAL(0.2579229713), FN_DECIMAL(0.7995216286), FN_DECIMAL(0.3928189967), FN_DECIMAL(0.04107517667), FN_DECIMAL(0.1534542912), FN_DECIMAL(0.6468965045), FN_DECIMAL(0.4030684878),
	FN_DECIMAL(-0.5617300988), FN_DECIMAL(-0.885463029), FN_DECIMAL(0.693729985), FN_DECIMAL(-0.5736527866), FN_DECIMAL(-0.9911905409), FN_DECIMAL(-0.66451538), FN_DECIMAL(0.2028855685), FN_DECIMAL(0.8019541421), FN_DECIMAL(-0.3903877149), FN_DECIMAL(-0.4888495114), FN_DECIMAL(-0.2753714057), FN_DECIMAL(-0.8915202143), FN_DECIMAL(0.5273119089), FN_DECIMAL(0.9363714773), FN_DECIMAL(-0.5212228249), FN_DECIMAL(-0.31642672),
	FN_DECIMAL(0.2409440761), FN_DECIMAL(-0.703776404), FN_DECIMAL(-0.6996810411), FN_DECIMAL(-0.7058714505), FN_DECIMAL(-0.3650566783), FN_DECIMAL(0.1064744278), FN_DECIMAL(0.7985729102), FN_DECIMAL(0.424680257), FN_DECIMAL(-0.6384535592), FN_DECIMAL(0.1540161646), FN_DECIMAL(-0.07702731943), FN_DECIMAL(-0.5627789132), FN_DECIMAL(-0.7667919169), FN_DECIMAL(-0.509815999), FN_DECIMAL(0.4590525092), FN_DECIMAL(0.1552595611),
	FN_DECIMAL(0.345402042), FN_DECIMAL(0.7537656024), FN_DECIMAL(0.7906259247), FN_DECIMAL(-0.6218493452), FN_DECIMAL(0.02979350071), FN_DECIMAL(-0.1337893489), FN_DECIMAL(-0.1483818606), FN_DECIMAL(0.549965562), FN_DECIMAL(0.01882482408), FN_DECIMAL(-0.7833783002), FN_DECIMAL(0.4702855809), FN_DECIMAL(0.2435827372), FN_DECIMAL(0.2978428332), FN_DECIMAL(0.2256499906), FN_DECIMAL(0.4885036897), FN_DECIMAL(0.5312962584),
	FN_DECIMAL(0.05401156992), FN_DECIMAL(0.1749922158), FN_DECIMAL(-0.7352273018), FN_DECIMAL(0.6058980284), FN_DECIMAL(0.4416079111), FN_DECIMAL(0.4417378638), FN_DECIMAL(0.5455879807), FN_DECIMAL(-0.6681295324), FN_DECIMAL(0.1973431441), FN_DECIMAL(0.4053292055), FN_DECIMAL(0.2220375492), FN_DECIMAL(0.2957118467), FN_DECIMAL(0.6910913512), FN_DECIMAL(0.5940890106), FN_DECIMAL(-0.2014135283), FN_DECIMAL(-0.9172588213),
	FN_DECIMAL(-0.4254361401), FN_DECIMAL(-0.6146586825), FN_DECIMAL(-0.7996193253), FN_DECIMAL(-0.3716777111), FN_DECIMAL(-0.9448876842), FN_DECIMAL(-0.2620349924), FN_DECIMAL(0.9615995749), FN_DECIMAL(-0.4679683524), FN_DECIMAL(0.3905937144), FN_DECIMAL(0.613593722), FN_DECIMAL(0.1422937358), FN_DECIMAL(0.1908754211), FN_DECIMAL(0.8189704912), FN_DECIMAL(-0.7300408736), FN_DECIMAL(-0.4108776451), FN_DECIMAL(-0.5319834504),
	FN_DECIMAL(-0.8970265651), FN_DECIMAL(-0.5386359045), FN_DECIMAL(0.4082255906), FN_DECIMAL(0.7245356676), FN_DECIMAL(0.5239080873), FN_DECIMAL(-0.8937552226), FN_DECIMAL(-0.553637673), FN_DECIMAL(0.2354455182), FN_DECIMAL(-0.0860293075), FN_DECIMAL(-0.1399373318), FN_DECIMAL(-0.4666323327), FN_DECIMAL(0.5560157407), FN_DECIMAL(0.1772619533), FN_DECIMAL(-0.8893937725), FN_DECIMAL(-0.5632714576), FN_DECIMAL(-0.5666264959),
	FN_DECIMAL(-0.3670263736), FN_DECIMAL(-0.06717242579), FN_DECIMAL(0.6205295181), FN_DECIMAL(-0.4110536264), FN_DECIMAL(0.7090054553), FN_DECIMAL(0.183899597), FN_DECIMAL(-0.5605470555), FN_DECIMAL(0.3879565548), FN_DECIMAL(0.7420893903), FN_DECIMAL(-0.2347595118), FN_DECIMAL(-0.8577217497), FN_DECIMAL(0.6325590203), FN_DECIMAL(-0.8736152276), FN_DECIMAL(0.7048011129), FN_DECIMAL(-0.06317948268), FN_DECIMAL(0.8753285574),
	FN_DECIMAL(-0.05843650473), FN_DECIMAL(-0.3674922622), FN_DECIMAL(-0.5256624401), FN_DECIMAL(0.7861039337), FN_DECIMAL(0.3287714416), FN_DECIMAL(0.5910593099), FN_DECIMAL(-0.3896960134), FN_DECIMAL(0.6864605361), FN_DECIMAL(0.7164918431), FN_DECIMAL(-0.290014277), FN_DECIMAL(-0.6796169617), FN_DECIMAL(0.1632515592), FN_DECIMAL(0.04485347486), FN_DECIMAL(0.8320545697), FN_DECIMAL(0.01339408056), FN_DECIMAL(-0.2874989857),
	FN_DECIMAL(0.615630723), FN_DECIMAL(0.3430367014), FN_DECIMAL(0.8193658136), FN_DECIMAL(-0.5829600957), FN_DECIMAL(0.07911697781), FN_DECIMAL(0.7854296063), FN_DECIMAL(-0.4107442306), FN_DECIMAL(0.4766964066), FN_DECIMAL(-0.9045999527), FN_DECIMAL(-0.1673856787), FN_DECIMAL(0.2828077348), FN_DECIMAL(-0.5902737632), FN_DECIMAL(-0.321506229), FN_DECIMAL(-0.5224513133), FN_DECIMAL(-0.4090169985), FN_DECIMAL(-0.3599685311),
};

static FORCEINLINE int FastFloor(FN_DECIMAL f) { return (f >= 0 ? (int)f : (int)f - 1); }
static FORCEINLINE int FastRound(FN_DECIMAL f) { return (f >= 0) ? (int)(f + FN_DECIMAL(0.5)) : (int)(f - FN_DECIMAL(0.5)); }
static FORCEINLINE int FastAbs(int i) { return abs(i); }
static FORCEINLINE FN_DECIMAL FastAbs(FN_DECIMAL f) { return fabs(f); }
static FORCEINLINE FN_DECIMAL FastAbsDeriv(FN_DECIMAL x, FN_DECIMAL dx) { return dx * (x < 0 ? -1 : 1); }
static FORCEINLINE FN_DECIMAL Lerp(FN_DECIMAL a, FN_DECIMAL b, FN_DECIMAL t) { return a + t * (b - a); }
static FORCEINLINE FN_DECIMAL InterpHermiteFunc(FN_DECIMAL t) { return t*t*(3 - 2 * t); }
static FORCEINLINE FN_DECIMAL InterpHermiteFuncDeriv(FN_DECIMAL t) { return -6 * t * (t - 1); }
static FORCEINLINE FN_DECIMAL InterpQuinticFunc(FN_DECIMAL t) { return t*t*t*(t*(t * 6 - 15) + 10); }
static FORCEINLINE FN_DECIMAL InterpQuinticFuncDeriv(FN_DECIMAL t) { return 30 * t * t * (t - 1) * (t - 1); }
static FORCEINLINE FN_DECIMAL CubicLerp(FN_DECIMAL a, FN_DECIMAL b, FN_DECIMAL c, FN_DECIMAL d, FN_DECIMAL t)
{
	FN_DECIMAL p = (d - c) - (a - b);
	return t * t * t * p + t * t * ((a - b) - p) + t * (c - a) + b;
}

void FastNoise::SetSeed(int seed)
{
	m_seed = seed;

	std::mt19937 gen(seed);

	for (int i = 0; i < 256; i++)
		m_perm[i] = i;

	for (int j = 0; j < 256; j++)
	{
		cross_platform_std::uniform_int_distribution<> dis(0, 256 - j);
		int k = dis(gen) + j;
		int l = m_perm[j];
		m_perm[j] = m_perm[j + 256] = m_perm[k];
		m_perm[k] = l;
		m_perm12[j] = m_perm12[j + 256] = m_perm[j] % 12;
	}
}

void FastNoise::CalculateFractalBounding(int octaves)
{
	float amp = m_gain;
	float ampFractal = 1.0f;
	for (int i = 1; i < octaves; i++)
	{
		ampFractal += amp;
		amp *= m_gain;
	}
	m_fractalBounding = 1.0f / ampFractal;
}

void FastNoise::SetCellularDistance2Indices(int cellularDistanceIndex0, int cellularDistanceIndex1)
{
	m_cellularDistanceIndex0 = std::min(cellularDistanceIndex0, cellularDistanceIndex1);
	m_cellularDistanceIndex1 = std::max(cellularDistanceIndex0, cellularDistanceIndex1);

	m_cellularDistanceIndex0 = std::min(std::max(m_cellularDistanceIndex0, 0), FN_CELLULAR_INDEX_MAX);
	m_cellularDistanceIndex1 = std::min(std::max(m_cellularDistanceIndex1, 0), FN_CELLULAR_INDEX_MAX);
}

void FastNoise::GetCellularDistance2Indices(int& cellularDistanceIndex0, int& cellularDistanceIndex1) const
{
	cellularDistanceIndex0 = m_cellularDistanceIndex0;
	cellularDistanceIndex1 = m_cellularDistanceIndex1;
}

FORCEINLINE unsigned char FastNoise::Index2D_12(unsigned char offset, int x, int y) const
{
	return m_perm12[(x & 0xff) + m_perm[(y & 0xff) + offset]];
}
FORCEINLINE unsigned char FastNoise::Index3D_12(unsigned char offset, int x, int y, int z) const
{
	return m_perm12[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + offset]]];
}
FORCEINLINE unsigned char FastNoise::Index4D_32(unsigned char offset, int x, int y, int z, int w) const
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + m_perm[(w & 0xff) + offset]]]] & 31;
}
FORCEINLINE unsigned char FastNoise::Index2D_256(unsigned char offset, int x, int y) const
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + offset]];
}
FORCEINLINE unsigned char FastNoise::Index3D_256(unsigned char offset, int x, int y, int z) const
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + offset]]];
}
FORCEINLINE unsigned char FastNoise::Index4D_256(unsigned char offset, int x, int y, int z, int w) const
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + m_perm[(w & 0xff) + offset]]]];
}

// Hashing
#define X_PRIME 1619
#define Y_PRIME 31337
#define Z_PRIME 6971
#define W_PRIME 1013

FORCEINLINE static FN_DECIMAL ValCoord2D(int seed, int x, int y)
{
	int n = seed;
	n ^= X_PRIME * x;
	n ^= Y_PRIME * y;

	return (n * n * n * 60493) / FN_DECIMAL(2147483648);
}
FORCEINLINE static FN_DECIMAL ValCoord3D(int seed, int x, int y, int z)
{
	int n = seed;
	n ^= X_PRIME * x;
	n ^= Y_PRIME * y;
	n ^= Z_PRIME * z;

	return (n * n * n * 60493) / FN_DECIMAL(2147483648);
}
FORCEINLINE static FN_DECIMAL ValCoord4D(int seed, int x, int y, int z, int w)
{
	int n = seed;
	n ^= X_PRIME * x;
	n ^= Y_PRIME * y;
	n ^= Z_PRIME * z;
	n ^= W_PRIME * w;

	return (n * n * n * 60493) / FN_DECIMAL(2147483648);
}

FORCEINLINE FN_DECIMAL FastNoise::ValCoord2DFast(unsigned char offset, int x, int y) const
{
	return VAL_LUT[Index2D_256(offset, x, y)];
}
FORCEINLINE FN_DECIMAL FastNoise::ValCoord3DFast(unsigned char offset, int x, int y, int z) const
{
	return VAL_LUT[Index3D_256(offset, x, y, z)];
}

FORCEINLINE FN_DECIMAL FastNoise::GradCoord2D(unsigned char offset, int x, int y, FN_DECIMAL xd, FN_DECIMAL yd) const
{
	unsigned char lutPos = Index2D_12(offset, x, y);

	return xd*GRAD_X[lutPos] + yd*GRAD_Y[lutPos];
}

FORCEINLINE FN_DECIMAL FastNoise::GradCoord2D(unsigned char offset, int x, int y, FN_DECIMAL xd, FN_DECIMAL yd, FN_DECIMAL& outGradX, FN_DECIMAL& outGradY) const
{
	unsigned char lutPos = Index2D_12(offset, x, y);

	outGradX = GRAD_X[lutPos];
	outGradY = GRAD_Y[lutPos];

	return xd*GRAD_X[lutPos] + yd*GRAD_Y[lutPos];
}

FN_DECIMAL FastNoise::GradCoord3D(unsigned char offset, int x, int y, int z, FN_DECIMAL xd, FN_DECIMAL yd, FN_DECIMAL zd) const
{
	unsigned char lutPos = Index3D_12(offset, x, y, z);

	return xd*GRAD_X[lutPos] + yd*GRAD_Y[lutPos] + zd*GRAD_Z[lutPos];
}

FN_DECIMAL FastNoise::GradCoord3D(unsigned char offset, int x, int y, int z, FN_DECIMAL xd, FN_DECIMAL yd, FN_DECIMAL zd, FN_DECIMAL& outGradX, FN_DECIMAL& outGradY, FN_DECIMAL& outGradZ) const
{
	unsigned char lutPos = Index3D_12(offset, x, y, z);
	
	outGradX = GRAD_X[lutPos];
	outGradY = GRAD_Y[lutPos];
	outGradZ = GRAD_Z[lutPos];

	return xd*GRAD_X[lutPos] + yd*GRAD_Y[lutPos] + zd*GRAD_Z[lutPos];
}

FN_DECIMAL FastNoise::GradCoord4D(unsigned char offset, int x, int y, int z, int w, FN_DECIMAL xd, FN_DECIMAL yd, FN_DECIMAL zd, FN_DECIMAL wd) const
{
	unsigned char lutPos = Index4D_32(offset, x, y, z, w) << 2;

	return xd*GRAD_4D[lutPos] + yd*GRAD_4D[lutPos + 1] + zd*GRAD_4D[lutPos + 2] + wd*GRAD_4D[lutPos + 3];
}
//
//FN_DECIMAL FastNoise::GetNoise(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency) const
//{
//	x *= frequency;
//	y *= frequency;
//	z *= frequency;
//
//	switch (m_noiseType)
//	{
//	case Value:
//		return SingleValue(0, x, y, z);
//	case ValueFractal:
//		switch (m_fractalType)
//		{
//		case FBM:
//			return SingleValueFractalFBM(x, y, z);
//		case Billow:
//			return SingleValueFractalBillow(x, y, z);
//		case RigidMulti:
//			return SingleValueFractalRigidMulti(x, y, z);
//		default:
//			return 0;
//		}
//	case Perlin:
//		return SinglePerlin(0, x, y, z);
//	case PerlinFractal:
//		switch (m_fractalType)
//		{
//		case FBM:
//			return SinglePerlinFractalFBM(x, y, z);
//		case Billow:
//			return SinglePerlinFractalBillow(x, y, z);
//		case RigidMulti:
//			return SinglePerlinFractalRigidMulti(x, y, z);
//		default:
//			return 0;
//		}
//	case Simplex:
//		return SingleSimplex(0, x, y, z);
//	case SimplexFractal:
//		switch (m_fractalType)
//		{
//		case FBM:
//			return SingleSimplexFractalFBM(x, y, z);
//		case Billow:
//			return SingleSimplexFractalBillow(x, y, z);
//		case RigidMulti:
//			return SingleSimplexFractalRigidMulti(x, y, z);
//		default:
//			return 0;
//		}
//	case Cellular:
//		switch (m_cellularReturnType)
//		{
//		case CellValue:
//		case NoiseLookup:
//		case Distance:
//			return SingleCellular(x, y, z);
//		default:
//			return SingleCellular2Edge(x, y, z);
//		}
//	case WhiteNoise:
//		return GetWhiteNoise(x, y, z);
//	case Cubic:
//		return SingleCubic(0, x, y, z);
//	case CubicFractal:
//		switch (m_fractalType)
//		{
//		case FBM:
//			return SingleCubicFractalFBM(x, y, z);
//		case Billow:
//			return SingleCubicFractalBillow(x, y, z);
//		case RigidMulti:
//			return SingleCubicFractalRigidMulti(x, y, z);
//		}
//	default:
//		return 0;
//	}
//}
//
//FN_DECIMAL FastNoise::GetNoise(FN_DECIMAL x, FN_DECIMAL y) const
//{
//	x *= frequency;
//	y *= frequency;
//
//	switch (m_noiseType)
//	{
//	case Value:
//		return SingleValue(0, x, y);
//	case ValueFractal:
//		switch (m_fractalType)
//		{
//		case FBM:
//			return SingleValueFractalFBM(x, y);
//		case Billow:
//			return SingleValueFractalBillow(x, y);
//		case RigidMulti:
//			return SingleValueFractalRigidMulti(x, y);
//		}
//	case Perlin:
//		return SinglePerlin(0, x, y);
//	case PerlinFractal:
//		switch (m_fractalType)
//		{
//		case FBM:
//			return SinglePerlinFractalFBM(x, y);
//		case Billow:
//			return SinglePerlinFractalBillow(x, y);
//		case RigidMulti:
//			return SinglePerlinFractalRigidMulti(x, y);
//		}
//	case Simplex:
//		return SingleSimplex(0, x, y);
//	case SimplexFractal:
//		switch (m_fractalType)
//		{
//		case FBM:
//			return SingleSimplexFractalFBM(x, y);
//		case Billow:
//			return SingleSimplexFractalBillow(x, y);
//		case RigidMulti:
//			return SingleSimplexFractalRigidMulti(x, y);
//		}
//	case Cellular:
//		switch (m_cellularReturnType)
//		{
//		case CellValue:
//		case NoiseLookup:
//		case Distance:
//			return SingleCellular(x, y);
//		default:
//			return SingleCellular2Edge(x, y);
//		}
//	case WhiteNoise:
//		return GetWhiteNoise(x, y);
//	case Cubic:
//		return SingleCubic(0, x, y);
//	case CubicFractal:
//		switch (m_fractalType)
//		{
//		case FBM:
//			return SingleCubicFractalFBM(x, y);
//		case Billow:
//			return SingleCubicFractalBillow(x, y);
//		case RigidMulti:
//			return SingleCubicFractalRigidMulti(x, y);
//		}
//	}
//	return 0;
//}

// White Noise
FN_DECIMAL FastNoise::GetWhiteNoise_4D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, FN_DECIMAL w) const
{
	return ValCoord4D(m_seed,
		*reinterpret_cast<int*>(&x) ^ (*reinterpret_cast<int*>(&x) >> 16),
		*reinterpret_cast<int*>(&y) ^ (*reinterpret_cast<int*>(&y) >> 16),
		*reinterpret_cast<int*>(&z) ^ (*reinterpret_cast<int*>(&z) >> 16),
		*reinterpret_cast<int*>(&w) ^ (*reinterpret_cast<int*>(&w) >> 16));
}

FN_DECIMAL FastNoise::GetWhiteNoise_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) const
{
	return ValCoord3D(m_seed,
		*reinterpret_cast<int*>(&x) ^ (*reinterpret_cast<int*>(&x) >> 16),
		*reinterpret_cast<int*>(&y) ^ (*reinterpret_cast<int*>(&y) >> 16),
		*reinterpret_cast<int*>(&z) ^ (*reinterpret_cast<int*>(&z) >> 16));
}

FN_DECIMAL FastNoise::GetWhiteNoise_2D(FN_DECIMAL x, FN_DECIMAL y) const
{
	return ValCoord2D(m_seed,
		*reinterpret_cast<int*>(&x) ^ (*reinterpret_cast<int*>(&x) >> 16),
		*reinterpret_cast<int*>(&y) ^ (*reinterpret_cast<int*>(&y) >> 16));
}

FN_DECIMAL FastNoise::GetWhiteNoiseInt_4D(int x, int y, int z, int w) const
{
	return ValCoord4D(m_seed, x, y, z, w);
}

FN_DECIMAL FastNoise::GetWhiteNoiseInt_3D(int x, int y, int z) const
{
	return ValCoord3D(m_seed, x, y, z);
}

FN_DECIMAL FastNoise::GetWhiteNoiseInt_2D(int x, int y) const
{
	return ValCoord2D(m_seed, x, y);
}

// Value Noise
FN_DECIMAL FastNoise::GetValueFractal_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency, int octaves) const
{
	x *= frequency;
	y *= frequency;
	z *= frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleValueFractalFBM_3D(x, y, z, octaves);
	case Billow:
		return SingleValueFractalBillow_3D(x, y, z, octaves);
	case RigidMulti:
		return SingleValueFractalRigidMulti_3D(x, y, z, octaves);
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::GetValueFractalDeriv_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	x *= frequency;
	y *= frequency;
	z *= frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleValueFractalDerivFBM_3D(x, y, z, octaves, outDx, outDy, outDz);
	case Billow:
		return SingleValueFractalDerivBillow_3D(x, y, z, octaves, outDx, outDy, outDz);
	case RigidMulti:
		return SingleValueFractalDerivRigidMulti_3D(x, y, z, octaves, outDx, outDy, outDz);
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::SingleValueFractalFBM_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves) const
{
	FN_DECIMAL sum = SingleValue_3D(m_perm[0], x, y, z);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += SingleValue_3D(m_perm[i], x, y, z) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleValueFractalBillow_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves) const
{
	FN_DECIMAL sum = FastAbs(SingleValue_3D(m_perm[0], x, y, z)) * 2 - 1;
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SingleValue_3D(m_perm[i], x, y, z)) * 2 - 1) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleValueFractalRigidMulti_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves) const
{
	FN_DECIMAL sum = 1 - FastAbs(SingleValue_3D(m_perm[0], x, y, z));
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum -= (1 - FastAbs(SingleValue_3D(m_perm[i], x, y, z))) * amp;
	}

	return sum;
}

FN_DECIMAL FastNoise::SingleValueFractalDerivFBM_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	FN_DECIMAL sum = SingleValueDeriv_3D(m_perm[0], x, y, z, outDx, outDy, outDz);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;

		FN_DECIMAL dx, dy, dz;
		sum += SingleValueDeriv_3D(m_perm[i], x, y, z, dx, dy, dz) * amp;
		outDx += amp * dx;
		outDy += amp * dy;
		outDz += amp * dz;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	outDz *= m_fractalBounding;
	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleValueFractalDerivBillow_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	FN_DECIMAL value, dx, dy, dz, sum;
	value = SingleValueDeriv_3D(m_perm[0], x, y, z, dx, dy, dz);

	sum = FastAbs(value) * 2 - 1;
	outDx = FastAbsDeriv(value, dx) * 2;
	outDy = FastAbsDeriv(value, dy) * 2;
	outDz = FastAbsDeriv(value, dz) * 2;

	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;
		amp *= m_gain;

		value = SingleValueDeriv_3D(m_perm[i], x, y, z, dx, dy, dz);

		sum += (FastAbs(value) * 2 - 1) * amp;
		outDx += FastAbsDeriv(value, dx) * 2 * amp;
		outDy += FastAbsDeriv(value, dy) * 2 * amp;
		outDz += FastAbsDeriv(value, dz) * 2 * amp;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	outDz *= m_fractalBounding;
	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleValueFractalDerivRigidMulti_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	FN_DECIMAL value, dx, dy, dz, sum;
	value = SingleValueDeriv_3D(m_perm[0], x, y, z, dx, dy, dz);

	sum =  1 - FastAbs(value);
	outDx = -FastAbsDeriv(value, dx);
	outDy = -FastAbsDeriv(value, dy);
	outDz = -FastAbsDeriv(value, dz);

	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;
		amp *= m_gain;

		value = SingleValueDeriv_3D(m_perm[i], x, y, z, dx, dy, dz);

		sum -= (1 - FastAbs(value)) * amp;
		outDx -= -FastAbsDeriv(value, dx) * amp;
		outDy -= -FastAbsDeriv(value, dy) * amp;
		outDz -= -FastAbsDeriv(value, dz) * amp;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	outDz *= m_fractalBounding;
	return sum;
}

FN_DECIMAL FastNoise::IQNoise_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency, int octaves) const
{
	FN_DECIMAL dx, dy, dz;
	return IQNoiseDeriv_3D(x, y, z, frequency, octaves, dx, dy, dz);
}

FN_DECIMAL FastNoise::IQNoiseDeriv_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	x *= frequency;
	y *= frequency;
	z *= frequency;

	FN_DECIMAL sum = SingleValueDeriv_3D(m_perm[0], x, y, z, outDx, outDy, outDz);
	FN_DECIMAL amp = 1;
	int i = 0;

	FN_DECIMAL localDx = outDx;
	FN_DECIMAL localDy = outDy;
	FN_DECIMAL localDz = outDz;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;
		
		FVector4 P = m_matrix3.TransformPosition({ float(x), float(y), float(z) });
		x = P.X;
		y = P.Y;
		z = P.Z;

		amp *= m_gain;

		FN_DECIMAL value, dx, dy, dz;
		value = SingleValueDeriv_3D(m_perm[i], x, y, z, dx, dy, dz);

		localDx += dx;
		localDy += dy;
		localDz += dz;

		FN_DECIMAL multiplier = amp / (1 + localDx * localDx + localDy * localDy + localDz * localDz);;
		sum += value * multiplier;

		// Not exact, but still gives good results
		outDx += dx * multiplier;
		outDy += dy * multiplier;
		outDy += dz * multiplier;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	outDz *= m_fractalBounding;
	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::GetValue_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency) const
{
	return SingleValue_3D(0, x * frequency, y * frequency, z * frequency);
}

FN_DECIMAL FastNoise::GetValueDeriv_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	return SingleValueDeriv_3D(0, x * frequency, y * frequency, z * frequency, outDx, outDy, outDz);
}

FN_DECIMAL FastNoise::SingleValue_3D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) const
{
	int x0 = FastFloor(x);
	int y0 = FastFloor(y);
	int z0 = FastFloor(z);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	int z1 = z0 + 1;

	FN_DECIMAL xs, ys, zs;
	switch (m_interp)
	{
	case Linear:
		xs = x - (FN_DECIMAL)x0;
		ys = y - (FN_DECIMAL)y0;
		zs = z - (FN_DECIMAL)z0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(x - (FN_DECIMAL)x0);
		ys = InterpHermiteFunc(y - (FN_DECIMAL)y0);
		zs = InterpHermiteFunc(z - (FN_DECIMAL)z0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(x - (FN_DECIMAL)x0);
		ys = InterpQuinticFunc(y - (FN_DECIMAL)y0);
		zs = InterpQuinticFunc(z - (FN_DECIMAL)z0);
		break;
	}

	FN_DECIMAL xf00 = Lerp(ValCoord3DFast(offset, x0, y0, z0), ValCoord3DFast(offset, x1, y0, z0), xs);
	FN_DECIMAL xf10 = Lerp(ValCoord3DFast(offset, x0, y1, z0), ValCoord3DFast(offset, x1, y1, z0), xs);
	FN_DECIMAL xf01 = Lerp(ValCoord3DFast(offset, x0, y0, z1), ValCoord3DFast(offset, x1, y0, z1), xs);
	FN_DECIMAL xf11 = Lerp(ValCoord3DFast(offset, x0, y1, z1), ValCoord3DFast(offset, x1, y1, z1), xs);

	FN_DECIMAL yf0 = Lerp(xf00, xf10, ys);
	FN_DECIMAL yf1 = Lerp(xf01, xf11, ys);

	return Lerp(yf0, yf1, zs);
}

FORCEINLINE FN_DECIMAL FastNoise::SingleValueDeriv_3D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	int x0 = FastFloor(x);
	int y0 = FastFloor(y);
	int z0 = FastFloor(z);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	int z1 = z0 + 1;
	FN_DECIMAL fx = x - x0;
	FN_DECIMAL fy = y - y0;
	FN_DECIMAL fz = z - z0;

	FN_DECIMAL xs, ys, zs;
	FN_DECIMAL dx, dy, dz;
	switch (m_interp)
	{
	case Linear:
		xs = fx;
		ys = fy;
		zs = fz;
		dx = 1;
		dy = 1;
		dz = 1;
		break;
	case Hermite:
		xs = InterpHermiteFunc(fx);
		ys = InterpHermiteFunc(fy);
		zs = InterpHermiteFunc(fz);
		dx = InterpHermiteFuncDeriv(fx);
		dy = InterpHermiteFuncDeriv(fy);
		dz = InterpHermiteFuncDeriv(fz);
		break;
	case Quintic:
		xs = InterpQuinticFunc(fx);
		ys = InterpQuinticFunc(fy);
		zs = InterpQuinticFunc(fz);
		dx = InterpQuinticFuncDeriv(fx);
		dy = InterpQuinticFuncDeriv(fy);
		dz = InterpQuinticFuncDeriv(fz);
		break;
	}

	FN_DECIMAL a = ValCoord3DFast(offset, x0, y0, z0);
	FN_DECIMAL b = ValCoord3DFast(offset, x1, y0, z0);
	FN_DECIMAL c = ValCoord3DFast(offset, x0, y1, z0);
	FN_DECIMAL d = ValCoord3DFast(offset, x1, y1, z0);
	FN_DECIMAL e = ValCoord3DFast(offset, x0, y0, z1);
	FN_DECIMAL f = ValCoord3DFast(offset, x1, y0, z1);
	FN_DECIMAL g = ValCoord3DFast(offset, x0, y1, z1);
	FN_DECIMAL h = ValCoord3DFast(offset, x1, y1, z1);

	FN_DECIMAL k0 = a;
	FN_DECIMAL k1 = b - a;
	FN_DECIMAL k2 = c - a;
	FN_DECIMAL k3 = e - a;
	FN_DECIMAL k4 = a - b - c + d;
	FN_DECIMAL k5 = a - c - e + g;
	FN_DECIMAL k6 = a - b - e + f;
	FN_DECIMAL k7 = -a + b + c - d + e - f - g + h;

	outDx = dx * (k1 + k4 * ys + k6 * zs + k7 * ys * zs);
	outDy = dy * (k2 + k5 * zs + k4 * xs + k7 * zs * xs);
	outDz = dz * (k3 + k6 * xs + k5 * ys + k7 * xs * ys);

	return k0 + k1 * xs + k2 * ys + k3 * zs + k4 * xs * ys + k5 * ys * zs + k6 * zs * xs + k7 * xs * ys * zs;
}

FN_DECIMAL FastNoise::GetValueFractal_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency, int octaves) const
{
	x *= frequency;
	y *= frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleValueFractalFBM_2D(x, y, octaves);
	case Billow:
		return SingleValueFractalBillow_2D(x, y, octaves);
	case RigidMulti:
		return SingleValueFractalRigidMulti_2D(x, y, octaves);
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::GetValueFractalDeriv_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	x *= frequency;
	y *= frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleValueFractalDerivFBM_2D(x, y, octaves, outDx, outDy);
	case Billow:
		return SingleValueFractalDerivBillow_2D(x, y, octaves, outDx, outDy);
	case RigidMulti:
		return SingleValueFractalDerivRigidMulti_2D(x, y, octaves, outDx, outDy);
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::SingleValueFractalFBM_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = SingleValue_2D(m_perm[0], x, y);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += SingleValue_2D(m_perm[i], x, y) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleValueFractalBillow_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = FastAbs(SingleValue_2D(m_perm[0], x, y)) * 2 - 1;
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		amp *= m_gain;
		sum += (FastAbs(SingleValue_2D(m_perm[i], x, y)) * 2 - 1) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleValueFractalRigidMulti_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = 1 - FastAbs(SingleValue_2D(m_perm[0], x, y));
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum -= (1 - FastAbs(SingleValue_2D(m_perm[i], x, y))) * amp;
	}

	return sum;
}

FN_DECIMAL FastNoise::SingleValueFractalDerivFBM_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	FN_DECIMAL sum = SingleValueDeriv_2D(m_perm[0], x, y, outDx, outDy);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;

		FN_DECIMAL dx, dy;
		sum += SingleValueDeriv_2D(m_perm[i], x, y, dx, dy) * amp;
		outDx += amp * dx;
		outDy += amp * dy;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleValueFractalDerivBillow_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	FN_DECIMAL value, dx, dy, sum;
	value = SingleValueDeriv_2D(m_perm[0], x, y, dx, dy);

	sum = FastAbs(value) * 2 - 1;
	outDx = FastAbsDeriv(value, dx) * 2;
	outDy = FastAbsDeriv(value, dy) * 2;

	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		amp *= m_gain;

		value = SingleValueDeriv_2D(m_perm[i], x, y, dx, dy);

		sum += (FastAbs(value) * 2 - 1) * amp;
		outDx += FastAbsDeriv(value, dx) * 2 * amp;
		outDy += FastAbsDeriv(value, dy) * 2 * amp;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleValueFractalDerivRigidMulti_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	FN_DECIMAL value, dx, dy, sum;
	value = SingleValueDeriv_2D(m_perm[0], x, y, dx, dy);

	sum =  1 - FastAbs(value);
	outDx = -FastAbsDeriv(value, dx);
	outDy = -FastAbsDeriv(value, dy);

	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		amp *= m_gain;

		value = SingleValueDeriv_2D(m_perm[i], x, y, dx, dy);

		sum -= (1 - FastAbs(value)) * amp;
		outDx -= -FastAbsDeriv(value, dx) * amp;
		outDy -= -FastAbsDeriv(value, dy) * amp;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	return sum;
}

FN_DECIMAL FastNoise::IQNoise_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency, int octaves) const
{
	FN_DECIMAL dx, dy;
	return IQNoiseDeriv_2D(x, y, frequency, octaves, dx, dy);
}

FN_DECIMAL FastNoise::IQNoiseDeriv_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	x *= frequency;
	y *= frequency;

	FN_DECIMAL sum = SingleValueDeriv_2D(m_perm[0], x, y, outDx, outDy);
	FN_DECIMAL amp = 1;
	int i = 0;

	FN_DECIMAL localDx = outDx;
	FN_DECIMAL localDy = outDy;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		FVector2D P = m_matrix2.TransformPoint({ float(x), float(y) });
		x = P.X;
		y = P.Y;

		amp *= m_gain;

		FN_DECIMAL value, dx, dy;
		value = SingleValueDeriv_2D(m_perm[i], x, y, dx, dy);

		localDx += dx;
		localDy += dy;

		FN_DECIMAL multiplier = amp / (1 + localDx * localDx + localDy * localDy);;
		sum += value * multiplier;

		// Not exact, but still gives good results
		outDx += dx * multiplier;
		outDy += dy * multiplier;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::GetValue_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency) const
{
	return SingleValue_2D(0, x * frequency, y * frequency);
}

FN_DECIMAL FastNoise::GetValueDeriv_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	return SingleValueDeriv_2D(0, x * frequency, y * frequency, outDx, outDy);
}

FN_DECIMAL FastNoise::SingleValue_2D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y) const
{
	int x0 = FastFloor(x);
	int y0 = FastFloor(y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	FN_DECIMAL xs, ys;
	switch (m_interp)
	{
	case Linear:
		xs = x - (FN_DECIMAL)x0;
		ys = y - (FN_DECIMAL)y0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(x - (FN_DECIMAL)x0);
		ys = InterpHermiteFunc(y - (FN_DECIMAL)y0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(x - (FN_DECIMAL)x0);
		ys = InterpQuinticFunc(y - (FN_DECIMAL)y0);
		break;
	}

	FN_DECIMAL xf0 = Lerp(ValCoord2DFast(offset, x0, y0), ValCoord2DFast(offset, x1, y0), xs);
	FN_DECIMAL xf1 = Lerp(ValCoord2DFast(offset, x0, y1), ValCoord2DFast(offset, x1, y1), xs);

	return Lerp(xf0, xf1, ys);
}

FN_DECIMAL FastNoise::SingleValueDeriv_2D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	int x0 = FastFloor(x);
	int y0 = FastFloor(y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	FN_DECIMAL fx = x - x0;
	FN_DECIMAL fy = y - y0;

	FN_DECIMAL xs, ys;
	FN_DECIMAL dx, dy;
	switch (m_interp)
	{
	case Linear:
		xs = fx;
		ys = fy;
		dx = 1;
		dy = 1;
		break;
	case Hermite:
		xs = InterpHermiteFunc(fx);
		ys = InterpHermiteFunc(fy);
		dx = InterpHermiteFuncDeriv(fx);
		dy = InterpHermiteFuncDeriv(fy);
		break;
	case Quintic:
		xs = InterpQuinticFunc(fx);
		ys = InterpQuinticFunc(fy);
		dx = InterpQuinticFuncDeriv(fx);
		dy = InterpQuinticFuncDeriv(fy);
		break;
	}

	FN_DECIMAL a = ValCoord2DFast(offset, x0, y0);
	FN_DECIMAL b = ValCoord2DFast(offset, x1, y0);
	FN_DECIMAL c = ValCoord2DFast(offset, x0, y1);
	FN_DECIMAL d = ValCoord2DFast(offset, x1, y1);

    FN_DECIMAL k0 = a;
    FN_DECIMAL k1 = b - a;
    FN_DECIMAL k2 = c - a;
    FN_DECIMAL k4 = a - b - c + d;
	
	outDx = dx * (ys * (a - b - c + d) + b - a);
	outDy = dy * (xs * (a - b - c + d) + c - a);

	return a + (b - a) * xs + (c - a) * ys + (a - b - c + d) * xs * ys;
}

// Perlin Noise
FN_DECIMAL FastNoise::GetPerlinFractal_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency, int octaves) const
{
	x *= frequency;
	y *= frequency;
	z *= frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SinglePerlinFractalFBM_3D(x, y, z, octaves);
	case Billow:
		return SinglePerlinFractalBillow_3D(x, y, z, octaves);
	case RigidMulti:
		return SinglePerlinFractalRigidMulti_3D(x, y, z, octaves);
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::GetPerlinFractalDeriv_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	x *= frequency;
	y *= frequency;
	z *= frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SinglePerlinFractalDerivFBM_3D(x, y, z, octaves, outDx, outDy, outDz);
	case Billow:
		return SinglePerlinFractalDerivBillow_3D(x, y, z, octaves, outDx, outDy, outDz);
	case RigidMulti:
		return SinglePerlinFractalDerivRigidMulti_3D(x, y, z, octaves, outDx, outDy, outDz);
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::SinglePerlinFractalFBM_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves) const
{
	FN_DECIMAL sum = SinglePerlin_3D(m_perm[0], x, y, z);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += SinglePerlin_3D(m_perm[i], x, y, z) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SinglePerlinFractalBillow_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves) const
{
	FN_DECIMAL sum = FastAbs(SinglePerlin_3D(m_perm[0], x, y, z)) * 2 - 1;
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SinglePerlin_3D(m_perm[i], x, y, z)) * 2 - 1) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SinglePerlinFractalRigidMulti_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves) const
{
	FN_DECIMAL sum = 1 - FastAbs(SinglePerlin_3D(m_perm[0], x, y, z));
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum -= (1 - FastAbs(SinglePerlin_3D(m_perm[i], x, y, z))) * amp;
	}

	return sum;
}

FN_DECIMAL FastNoise::SinglePerlinFractalDerivFBM_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	FN_DECIMAL sum = SinglePerlinDeriv_3D(m_perm[0], x, y, z, outDx, outDy, outDz);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;

		FN_DECIMAL dx, dy, dz;
		sum += SinglePerlinDeriv_3D(m_perm[i], x, y, z, dx, dy, dz) * amp;
		outDx += amp * dx;
		outDy += amp * dy;
		outDz += amp * dz;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	outDz *= m_fractalBounding;
	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SinglePerlinFractalDerivBillow_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	FN_DECIMAL value, dx, dy, dz, sum;
	value = SinglePerlinDeriv_3D(m_perm[0], x, y, z, dx, dy, dz);

	sum = FastAbs(value) * 2 - 1;
	outDx = FastAbsDeriv(value, dx) * 2;
	outDy = FastAbsDeriv(value, dy) * 2;
	outDz = FastAbsDeriv(value, dz) * 2;

	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;
		amp *= m_gain;

		value = SinglePerlinDeriv_3D(m_perm[i], x, y, z, dx, dy, dz);

		sum += (FastAbs(value) * 2 - 1) * amp;
		outDx += FastAbsDeriv(value, dx) * 2 * amp;
		outDy += FastAbsDeriv(value, dy) * 2 * amp;
		outDz += FastAbsDeriv(value, dz) * 2 * amp;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	outDz *= m_fractalBounding;
	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SinglePerlinFractalDerivRigidMulti_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	FN_DECIMAL value, dx, dy, dz, sum;
	value = SinglePerlinDeriv_3D(m_perm[0], x, y, z, dx, dy, dz);

	sum =  1 - FastAbs(value);
	outDx = -FastAbsDeriv(value, dx);
	outDy = -FastAbsDeriv(value, dy);
	outDz = -FastAbsDeriv(value, dz);

	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;
		amp *= m_gain;

		value = SinglePerlinDeriv_3D(m_perm[i], x, y, z, dx, dy, dz);

		sum -= (1 - FastAbs(value)) * amp;
		outDx -= -FastAbsDeriv(value, dx) * amp;
		outDy -= -FastAbsDeriv(value, dy) * amp;
		outDz -= -FastAbsDeriv(value, dz) * amp;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	outDz *= m_fractalBounding;
	return sum;
}

FN_DECIMAL FastNoise::GetPerlin_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency) const
{
	return SinglePerlin_3D(0, x * frequency, y * frequency, z * frequency);
}

FN_DECIMAL FastNoise::GetPerlinDeriv_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	return SinglePerlinDeriv_3D(0, x * frequency, y * frequency, z * frequency, outDx, outDy, outDz);
}

FN_DECIMAL FastNoise::SinglePerlin_3D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) const
{
	int x0 = FastFloor(x);
	int y0 = FastFloor(y);
	int z0 = FastFloor(z);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	int z1 = z0 + 1;

	FN_DECIMAL xs, ys, zs;
	switch (m_interp)
	{
	case Linear:
		xs = x - (FN_DECIMAL)x0;
		ys = y - (FN_DECIMAL)y0;
		zs = z - (FN_DECIMAL)z0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(x - (FN_DECIMAL)x0);
		ys = InterpHermiteFunc(y - (FN_DECIMAL)y0);
		zs = InterpHermiteFunc(z - (FN_DECIMAL)z0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(x - (FN_DECIMAL)x0);
		ys = InterpQuinticFunc(y - (FN_DECIMAL)y0);
		zs = InterpQuinticFunc(z - (FN_DECIMAL)z0);
		break;
	}

	FN_DECIMAL xd0 = x - (FN_DECIMAL)x0;
	FN_DECIMAL yd0 = y - (FN_DECIMAL)y0;
	FN_DECIMAL zd0 = z - (FN_DECIMAL)z0;
	FN_DECIMAL xd1 = xd0 - 1;
	FN_DECIMAL yd1 = yd0 - 1;
	FN_DECIMAL zd1 = zd0 - 1;

	FN_DECIMAL xf00 = Lerp(GradCoord3D(offset, x0, y0, z0, xd0, yd0, zd0), GradCoord3D(offset, x1, y0, z0, xd1, yd0, zd0), xs);
	FN_DECIMAL xf10 = Lerp(GradCoord3D(offset, x0, y1, z0, xd0, yd1, zd0), GradCoord3D(offset, x1, y1, z0, xd1, yd1, zd0), xs);
	FN_DECIMAL xf01 = Lerp(GradCoord3D(offset, x0, y0, z1, xd0, yd0, zd1), GradCoord3D(offset, x1, y0, z1, xd1, yd0, zd1), xs);
	FN_DECIMAL xf11 = Lerp(GradCoord3D(offset, x0, y1, z1, xd0, yd1, zd1), GradCoord3D(offset, x1, y1, z1, xd1, yd1, zd1), xs);

	FN_DECIMAL yf0 = Lerp(xf00, xf10, ys);
	FN_DECIMAL yf1 = Lerp(xf01, xf11, ys);

	return Lerp(yf0, yf1, zs);
}

FN_DECIMAL FastNoise::SinglePerlinDeriv_3D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, FN_DECIMAL& outDx, FN_DECIMAL& outDy, FN_DECIMAL& outDz) const
{
	int x0 = FastFloor(x);
	int y0 = FastFloor(y);
	int z0 = FastFloor(z);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	int z1 = z0 + 1;
	FN_DECIMAL fx = x - x0;
	FN_DECIMAL fy = y - y0;
	FN_DECIMAL fz = z - z0;
	
	FN_DECIMAL xs, ys, zs;
	FN_DECIMAL dx, dy, dz;
	switch (m_interp)
	{
	case Linear:
		xs = fx;
		ys = fy;
		zs = fz;
		dx = 1;
		dy = 1;
		dz = 1;
		break;
	case Hermite:
		xs = InterpHermiteFunc(fx);
		ys = InterpHermiteFunc(fy);
		zs = InterpHermiteFunc(fz);
		dx = InterpHermiteFuncDeriv(fx);
		dy = InterpHermiteFuncDeriv(fy);
		dz = InterpHermiteFuncDeriv(fz);
		break;
	case Quintic:
		xs = InterpQuinticFunc(fx);
		ys = InterpQuinticFunc(fy);
		zs = InterpQuinticFunc(fz);
		dx = InterpQuinticFuncDeriv(fx);
		dy = InterpQuinticFuncDeriv(fy);
		dz = InterpQuinticFuncDeriv(fz);
		break;
	}

	FN_DECIMAL xd0 = fx;
	FN_DECIMAL yd0 = fy;
	FN_DECIMAL zd0 = fz;
	FN_DECIMAL xd1 = xd0 - 1;
	FN_DECIMAL yd1 = yd0 - 1;
	FN_DECIMAL zd1 = zd0 - 1;
	
    FN_DECIMAL gax, gay, gaz;
    FN_DECIMAL gbx, gby, gbz;
    FN_DECIMAL gcx, gcy, gcz;
    FN_DECIMAL gdx, gdy, gdz;
    FN_DECIMAL gex, gey, gez;
    FN_DECIMAL gfx, gfy, gfz;
    FN_DECIMAL ggx, ggy, ggz;
    FN_DECIMAL ghx, ghy, ghz;
    
    FN_DECIMAL va = GradCoord3D(offset, x0, y0, z0, xd0, yd0, zd0, gax, gay, gaz);
    FN_DECIMAL vb = GradCoord3D(offset, x1, y0, z0, xd1, yd0, zd0, gbx, gby, gbz);
    FN_DECIMAL vc = GradCoord3D(offset, x0, y1, z0, xd0, yd1, zd0, gcx, gcy, gcz);
    FN_DECIMAL vd = GradCoord3D(offset, x1, y1, z0, xd1, yd1, zd0, gdx, gdy, gdz);
    FN_DECIMAL ve = GradCoord3D(offset, x0, y0, z1, xd0, yd0, zd1, gex, gey, gez);
    FN_DECIMAL vf = GradCoord3D(offset, x1, y0, z1, xd1, yd0, zd1, gfx, gfy, gfz);
    FN_DECIMAL vg = GradCoord3D(offset, x0, y1, z1, xd0, yd1, zd1, ggx, ggy, ggz);
	FN_DECIMAL vh = GradCoord3D(offset, x1, y1, z1, xd1, yd1, zd1, ghx, ghy, ghz);

	outDx =
		gax +
		xs * (gbx - gax) +
		ys * (gcx - gax) +
		zs * (gex - gax) +
		xs * ys * (gax - gbx - gcx + gdx) +
		ys * zs * (gax - gcx - gex + ggx) +
		zs * xs * (gax - gbx - gex + gfx) +
		xs * ys * zs * (-gax + gbx + gcx - gdx + gex - gfx - ggx + ghx) +

		dx * (
			vb - va +
			ys * (va - vb - vc + vd) +
			zs * (va - vb - ve + vf) +
			ys * zs * (-va + vb + vc - vd + ve - vf - vg + vh));

	outDy =
		gay +
		xs * (gby - gay) +
		ys * (gcy - gay) +
		zs * (gey - gay) +
		xs * ys * (gay - gby - gcy + gdy) +
		ys * zs * (gay - gcy - gey + ggy) +
		zs * xs * (gay - gby - gey + gfy) +
		xs * ys * zs * (-gay + gby + gcy - gdy + gey - gfy - ggy + ghy) +

		dy * (
			vc - va +
			zs * (va - vc - ve + vg) +
			xs * (va - vb - vc + vd) +
			zs * xs * (-va + vb + vc - vd + ve - vf - vg + vh));

	outDz =
		gaz +
		xs * (gbz - gaz) +
		ys * (gcz - gaz) +
		zs * (gez - gaz) +
		xs * ys * (gaz - gbz - gcz + gdz) +
		ys * zs * (gaz - gcz - gez + ggz) +
		zs * xs * (gaz - gbz - gez + gfz) +
		xs * ys * zs * (-gaz + gbz + gcz - gdz + gez - gfz - ggz + ghz) +

		dz * (
			ve - va +
			xs * (va - vb - ve + vf) +
			ys * (va - vc - ve + vg) +
			xs * ys * (-va + vb + vc - vd + ve - vf - vg + vh));

	return 
		va + 
		xs * (vb - va) + 
		ys * (vc - va) + 
		zs * (ve - va) + 
		xs * ys * (va - vb - vc + vd) + 
		ys * zs * (va - vc - ve + vg) + 
		zs * xs * (va - vb - ve + vf) + 
		xs * ys * zs * (-va + vb + vc - vd + ve - vf - vg + vh);
}

FN_DECIMAL FastNoise::GetPerlinFractal_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency, int octaves) const
{
	x *= frequency;
	y *= frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SinglePerlinFractalFBM_2D(x, y, octaves);
	case Billow:
		return SinglePerlinFractalBillow_2D(x, y, octaves);
	case RigidMulti:
		return SinglePerlinFractalRigidMulti_2D(x, y, octaves);
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::GetPerlinFractalDeriv_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	x *= frequency;
	y *= frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SinglePerlinFractalDerivFBM_2D(x, y, octaves, outDx, outDy);
	case Billow:
		return SinglePerlinFractalDerivBillow_2D(x, y, octaves, outDx, outDy);
	case RigidMulti:
		return SinglePerlinFractalDerivRigidMulti_2D(x, y, octaves, outDx, outDy);
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::SinglePerlinFractalFBM_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = SinglePerlin_2D(m_perm[0], x, y);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += SinglePerlin_2D(m_perm[i], x, y) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SinglePerlinFractalBillow_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = FastAbs(SinglePerlin_2D(m_perm[0], x, y)) * 2 - 1;
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SinglePerlin_2D(m_perm[i], x, y)) * 2 - 1) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SinglePerlinFractalRigidMulti_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = 1 - FastAbs(SinglePerlin_2D(m_perm[0], x, y));
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum -= (1 - FastAbs(SinglePerlin_2D(m_perm[i], x, y))) * amp;
	}

	return sum;
}

FN_DECIMAL FastNoise::SinglePerlinFractalDerivFBM_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	FN_DECIMAL sum = SinglePerlinDeriv_2D(m_perm[0], x, y, outDx, outDy);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;

		FN_DECIMAL dx, dy;
		sum += SinglePerlinDeriv_2D(m_perm[i], x, y, dx, dy) * amp;
		outDx += amp * dx;
		outDy += amp * dy;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SinglePerlinFractalDerivBillow_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	FN_DECIMAL value, dx, dy, sum;
	value = SinglePerlinDeriv_2D(m_perm[0], x, y, dx, dy);

	sum = FastAbs(value) * 2 - 1;
	outDx = FastAbsDeriv(value, dx) * 2;
	outDy = FastAbsDeriv(value, dy) * 2;

	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		amp *= m_gain;

		value = SinglePerlinDeriv_2D(m_perm[i], x, y, dx, dy);

		sum += (FastAbs(value) * 2 - 1) * amp;
		outDx += FastAbsDeriv(value, dx) * 2 * amp;
		outDy += FastAbsDeriv(value, dy) * 2 * amp;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SinglePerlinFractalDerivRigidMulti_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	FN_DECIMAL value, dx, dy, sum;
	value = SinglePerlinDeriv_2D(m_perm[0], x, y, dx, dy);

	sum = 1 - FastAbs(value);
	outDx = -FastAbsDeriv(value, dx);
	outDy = -FastAbsDeriv(value, dy);

	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		amp *= m_gain;

		value = SinglePerlinDeriv_2D(m_perm[i], x, y, dx, dy);

		sum -= (1 - FastAbs(value)) * amp;
		outDx -= -FastAbsDeriv(value, dx) * amp;
		outDy -= -FastAbsDeriv(value, dy) * amp;
	}

	outDx *= m_fractalBounding;
	outDy *= m_fractalBounding;
	return sum;
}

FN_DECIMAL FastNoise::GetPerlin_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency) const
{
	return SinglePerlin_2D(0, x * frequency, y * frequency);
}

FN_DECIMAL FastNoise::GetPerlinDeriv_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	return SinglePerlinDeriv_2D(0, x * frequency, y * frequency, outDx, outDy);
}

FN_DECIMAL FastNoise::SinglePerlin_2D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y) const
{
	int x0 = FastFloor(x);
	int y0 = FastFloor(y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	FN_DECIMAL xs, ys;
	switch (m_interp)
	{
	case Linear:
		xs = x - (FN_DECIMAL)x0;
		ys = y - (FN_DECIMAL)y0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(x - (FN_DECIMAL)x0);
		ys = InterpHermiteFunc(y - (FN_DECIMAL)y0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(x - (FN_DECIMAL)x0);
		ys = InterpQuinticFunc(y - (FN_DECIMAL)y0);
		break;
	}

	FN_DECIMAL xd0 = x - (FN_DECIMAL)x0;
	FN_DECIMAL yd0 = y - (FN_DECIMAL)y0;
	FN_DECIMAL xd1 = xd0 - 1;
	FN_DECIMAL yd1 = yd0 - 1;

	FN_DECIMAL xf0 = Lerp(GradCoord2D(offset, x0, y0, xd0, yd0), GradCoord2D(offset, x1, y0, xd1, yd0), xs);
	FN_DECIMAL xf1 = Lerp(GradCoord2D(offset, x0, y1, xd0, yd1), GradCoord2D(offset, x1, y1, xd1, yd1), xs);

	return Lerp(xf0, xf1, ys);
}

FN_DECIMAL FastNoise::SinglePerlinDeriv_2D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL& outDx, FN_DECIMAL& outDy) const
{
	int x0 = FastFloor(x);
	int y0 = FastFloor(y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	FN_DECIMAL fx = x - x0;
	FN_DECIMAL fy = y - y0;
	
	FN_DECIMAL xs, ys;
	FN_DECIMAL dx, dy;
	switch (m_interp)
	{
	case Linear:
		xs = fx;
		ys = fy;
		dx = 1;
		dy = 1;
		break;
	case Hermite:
		xs = InterpHermiteFunc(fx);
		ys = InterpHermiteFunc(fy);
		dx = InterpHermiteFuncDeriv(fx);
		dy = InterpHermiteFuncDeriv(fy);
		break;
	case Quintic:
		xs = InterpQuinticFunc(fx);
		ys = InterpQuinticFunc(fy);
		dx = InterpQuinticFuncDeriv(fx);
		dy = InterpQuinticFuncDeriv(fy);
		break;
	}

	FN_DECIMAL xd0 = fx;
	FN_DECIMAL yd0 = fy;
	FN_DECIMAL xd1 = xd0 - 1;
	FN_DECIMAL yd1 = yd0 - 1;
	
    FN_DECIMAL gax, gay;
    FN_DECIMAL gbx, gby;
    FN_DECIMAL gcx, gcy;
    FN_DECIMAL gdx, gdy;
    
    FN_DECIMAL va = GradCoord2D(offset, x0, y0, xd0, yd0, gax, gay);
    FN_DECIMAL vb = GradCoord2D(offset, x1, y0, xd1, yd0, gbx, gby);
    FN_DECIMAL vc = GradCoord2D(offset, x0, y1, xd0, yd1, gcx, gcy);
    FN_DECIMAL vd = GradCoord2D(offset, x1, y1, xd1, yd1, gdx, gdy);

	outDx = gax + xs * (gbx - gax) + ys * (gcx - gax) + xs * ys * (gax - gbx - gcx + gdx) + dx * (ys * (va - vb - vc + vd) + vb - va);
	outDy = gay + xs * (gby - gay) + ys * (gcy - gay) + xs * ys * (gay - gby - gcy + gdy) + dy * (xs * (va - vb - vc + vd) + vc - va);

	return va + xs * (vb - va) + ys * (vc - va) + xs * ys * (va - vb - vc + vd);
}

// Simplex Noise

FN_DECIMAL FastNoise::GetSimplexFractal_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency, int octaves) const
{
	x *= frequency;
	y *= frequency;
	z *= frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleSimplexFractalFBM_3D(x, y, z, octaves);
	case Billow:
		return SingleSimplexFractalBillow_3D(x, y, z, octaves);
	case RigidMulti:
		return SingleSimplexFractalRigidMulti_3D(x, y, z, octaves);
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::SingleSimplexFractalFBM_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves) const
{
	FN_DECIMAL sum = SingleSimplex_3D(m_perm[0], x, y, z);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += SingleSimplex_3D(m_perm[i], x, y, z) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleSimplexFractalBillow_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves) const
{
	FN_DECIMAL sum = FastAbs(SingleSimplex_3D(m_perm[0], x, y, z)) * 2 - 1;
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SingleSimplex_3D(m_perm[i], x, y, z)) * 2 - 1) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleSimplexFractalRigidMulti_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves) const
{
	FN_DECIMAL sum = 1 - FastAbs(SingleSimplex_3D(m_perm[0], x, y, z));
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum -= (1 - FastAbs(SingleSimplex_3D(m_perm[i], x, y, z))) * amp;
	}

	return sum;
}

FN_DECIMAL FastNoise::GetSimplex_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency) const
{
	return SingleSimplex_3D(0, x * frequency, y * frequency, z * frequency);
}

static const FN_DECIMAL F3 = 1 / FN_DECIMAL(3);
static const FN_DECIMAL G3 = 1 / FN_DECIMAL(6);

FN_DECIMAL FastNoise::SingleSimplex_3D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) const
{
	FN_DECIMAL t = (x + y + z) * F3;
	int i = FastFloor(x + t);
	int j = FastFloor(y + t);
	int k = FastFloor(z + t);

	t = (i + j + k) * G3;
	FN_DECIMAL X0 = i - t;
	FN_DECIMAL Y0 = j - t;
	FN_DECIMAL Z0 = k - t;

	FN_DECIMAL x0 = x - X0;
	FN_DECIMAL y0 = y - Y0;
	FN_DECIMAL z0 = z - Z0;

	int i1, j1, k1;
	int i2, j2, k2;

	if (x0 >= y0)
	{
		if (y0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		}
		else if (x0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1;
		}
		else // x0 < z0
		{
			i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1;
		}
	}
	else // x0 < y0
	{
		if (y0 < z0)
		{
			i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1;
		}
		else if (x0 < z0)
		{
			i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1;
		}
		else // x0 >= z0
		{
			i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		}
	}

	FN_DECIMAL x1 = x0 - i1 + G3;
	FN_DECIMAL y1 = y0 - j1 + G3;
	FN_DECIMAL z1 = z0 - k1 + G3;
	FN_DECIMAL x2 = x0 - i2 + 2 * G3;
	FN_DECIMAL y2 = y0 - j2 + 2 * G3;
	FN_DECIMAL z2 = z0 - k2 + 2 * G3;
	FN_DECIMAL x3 = x0 - 1 + 3 * G3;
	FN_DECIMAL y3 = y0 - 1 + 3 * G3;
	FN_DECIMAL z3 = z0 - 1 + 3 * G3;

	FN_DECIMAL n0, n1, n2, n3;

	t = FN_DECIMAL(0.6) - x0*x0 - y0*y0 - z0*z0;
	if (t < 0) n0 = 0;
	else
	{
		t *= t;
		n0 = t*t*GradCoord3D(offset, i, j, k, x0, y0, z0);
	}

	t = FN_DECIMAL(0.6) - x1*x1 - y1*y1 - z1*z1;
	if (t < 0) n1 = 0;
	else
	{
		t *= t;
		n1 = t*t*GradCoord3D(offset, i + i1, j + j1, k + k1, x1, y1, z1);
	}

	t = FN_DECIMAL(0.6) - x2*x2 - y2*y2 - z2*z2;
	if (t < 0) n2 = 0;
	else
	{
		t *= t;
		n2 = t*t*GradCoord3D(offset, i + i2, j + j2, k + k2, x2, y2, z2);
	}

	t = FN_DECIMAL(0.6) - x3*x3 - y3*y3 - z3*z3;
	if (t < 0) n3 = 0;
	else
	{
		t *= t;
		n3 = t*t*GradCoord3D(offset, i + 1, j + 1, k + 1, x3, y3, z3);
	}

	return 32 * (n0 + n1 + n2 + n3);
}

FN_DECIMAL FastNoise::GetSimplexFractal_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency, int octaves) const
{
	x *= frequency;
	y *= frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleSimplexFractalFBM_2D(x, y, octaves);
	case Billow:
		return SingleSimplexFractalBillow_2D(x, y, octaves);
	case RigidMulti:
		return SingleSimplexFractalRigidMulti_2D(x, y, octaves);
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::SingleSimplexFractalFBM_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = SingleSimplex_2D(m_perm[0], x, y);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += SingleSimplex_2D(m_perm[i], x, y) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleSimplexFractalBillow_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = FastAbs(SingleSimplex_2D(m_perm[0], x, y)) * 2 - 1;
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SingleSimplex_2D(m_perm[i], x, y)) * 2 - 1) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleSimplexFractalRigidMulti_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = 1 - FastAbs(SingleSimplex_2D(m_perm[0], x, y));
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum -= (1 - FastAbs(SingleSimplex_2D(m_perm[i], x, y))) * amp;
	}

	return sum;
}

FN_DECIMAL FastNoise::SingleSimplexFractalBlend_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = SingleSimplex_2D(m_perm[0], x, y);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum *= SingleSimplex_2D(m_perm[i], x, y) * amp + 1;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::GetSimplex_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency) const
{
	return SingleSimplex_2D(0, x * frequency, y * frequency);
}

//static const FN_DECIMAL F2 = 1 / FN_DECIMAL(2);
//static const FN_DECIMAL G2 = 1 / FN_DECIMAL(4);

static const FN_DECIMAL SQRT3 = FN_DECIMAL(1.7320508075688772935274463415059);
static const FN_DECIMAL F2 = FN_DECIMAL(0.5) * (SQRT3 - FN_DECIMAL(1.0));
static const FN_DECIMAL G2 = (FN_DECIMAL(3.0) - SQRT3) / FN_DECIMAL(6.0);

FN_DECIMAL FastNoise::SingleSimplex_2D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y) const
{
	FN_DECIMAL t = (x + y) * F2;
	int i = FastFloor(x + t);
	int j = FastFloor(y + t);

	t = (i + j) * G2;
	FN_DECIMAL X0 = i - t;
	FN_DECIMAL Y0 = j - t;

	FN_DECIMAL x0 = x - X0;
	FN_DECIMAL y0 = y - Y0;

	int i1, j1;
	if (x0 > y0)
	{
		i1 = 1; j1 = 0;
	}
	else
	{
		i1 = 0; j1 = 1;
	}

	FN_DECIMAL x1 = x0 - (FN_DECIMAL)i1 + G2;
	FN_DECIMAL y1 = y0 - (FN_DECIMAL)j1 + G2;
	FN_DECIMAL x2 = x0 - 1 + 2 * G2;
	FN_DECIMAL y2 = y0 - 1 + 2 * G2;

	FN_DECIMAL n0, n1, n2;

	t = FN_DECIMAL(0.5) - x0*x0 - y0*y0;
	if (t < 0) n0 = 0;
	else
	{
		t *= t;
		n0 = t * t * GradCoord2D(offset, i, j, x0, y0);
	}

	t = FN_DECIMAL(0.5) - x1*x1 - y1*y1;
	if (t < 0) n1 = 0;
	else
	{
		t *= t;
		n1 = t*t*GradCoord2D(offset, i + i1, j + j1, x1, y1);
	}

	t = FN_DECIMAL(0.5) - x2*x2 - y2*y2;
	if (t < 0) n2 = 0;
	else
	{
		t *= t;
		n2 = t*t*GradCoord2D(offset, i + 1, j + 1, x2, y2);
	}

	return 70 * (n0 + n1 + n2);
}

FN_DECIMAL FastNoise::GetSimplex_4D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, FN_DECIMAL w, float frequency) const
{
	return SingleSimplex_4D(0, x * frequency, y * frequency, z * frequency, w * frequency);
}

static const unsigned char SIMPLEX_4D[] =
{
	0,1,2,3,0,1,3,2,0,0,0,0,0,2,3,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,0,
	0,2,1,3,0,0,0,0,0,3,1,2,0,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,3,2,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,2,0,3,0,0,0,0,1,3,0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,3,0,1,2,3,1,0,
	1,0,2,3,1,0,3,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,3,1,0,0,0,0,2,1,3,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	2,0,1,3,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1,2,3,0,2,1,0,0,0,0,3,1,2,0,
	2,1,0,3,0,0,0,0,0,0,0,0,0,0,0,0,3,1,0,2,0,0,0,0,3,2,0,1,3,2,1,0
};

static const FN_DECIMAL F4 = (sqrt(FN_DECIMAL(5)) - 1) / 4;
static const FN_DECIMAL G4 = (5 - sqrt(FN_DECIMAL(5))) / 2;

FN_DECIMAL FastNoise::SingleSimplex_4D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, FN_DECIMAL w) const
{
	FN_DECIMAL n0, n1, n2, n3, n4;
	FN_DECIMAL t = (x + y + z + w) * F4;
	int i = FastFloor(x + t);
	int j = FastFloor(y + t);
	int k = FastFloor(z + t);
	int l = FastFloor(w + t);
	t = (i + j + k + l) * G4;
	FN_DECIMAL X0 = i - t;
	FN_DECIMAL Y0 = j - t;
	FN_DECIMAL Z0 = k - t;
	FN_DECIMAL W0 = l - t;
	FN_DECIMAL x0 = x - X0;
	FN_DECIMAL y0 = y - Y0;
	FN_DECIMAL z0 = z - Z0;
	FN_DECIMAL w0 = w - W0;

	int c = (x0 > y0) ? 32 : 0;
	c += (x0 > z0) ? 16 : 0;
	c += (y0 > z0) ? 8 : 0;
	c += (x0 > w0) ? 4 : 0;
	c += (y0 > w0) ? 2 : 0;
	c += (z0 > w0) ? 1 : 0;
	c <<= 2;

	int i1 = SIMPLEX_4D[c] >= 3 ? 1 : 0;
	int i2 = SIMPLEX_4D[c] >= 2 ? 1 : 0;
	int i3 = SIMPLEX_4D[c++] >= 1 ? 1 : 0;
	int j1 = SIMPLEX_4D[c] >= 3 ? 1 : 0;
	int j2 = SIMPLEX_4D[c] >= 2 ? 1 : 0;
	int j3 = SIMPLEX_4D[c++] >= 1 ? 1 : 0;
	int k1 = SIMPLEX_4D[c] >= 3 ? 1 : 0;
	int k2 = SIMPLEX_4D[c] >= 2 ? 1 : 0;
	int k3 = SIMPLEX_4D[c++] >= 1 ? 1 : 0;
	int l1 = SIMPLEX_4D[c] >= 3 ? 1 : 0;
	int l2 = SIMPLEX_4D[c] >= 2 ? 1 : 0;
	int l3 = SIMPLEX_4D[c] >= 1 ? 1 : 0;

	FN_DECIMAL x1 = x0 - i1 + G4;
	FN_DECIMAL y1 = y0 - j1 + G4;
	FN_DECIMAL z1 = z0 - k1 + G4;
	FN_DECIMAL w1 = w0 - l1 + G4;
	FN_DECIMAL x2 = x0 - i2 + 2 * G4;
	FN_DECIMAL y2 = y0 - j2 + 2 * G4;
	FN_DECIMAL z2 = z0 - k2 + 2 * G4;
	FN_DECIMAL w2 = w0 - l2 + 2 * G4;
	FN_DECIMAL x3 = x0 - i3 + 3 * G4;
	FN_DECIMAL y3 = y0 - j3 + 3 * G4;
	FN_DECIMAL z3 = z0 - k3 + 3 * G4;
	FN_DECIMAL w3 = w0 - l3 + 3 * G4;
	FN_DECIMAL x4 = x0 - 1 + 4 * G4;
	FN_DECIMAL y4 = y0 - 1 + 4 * G4;
	FN_DECIMAL z4 = z0 - 1 + 4 * G4;
	FN_DECIMAL w4 = w0 - 1 + 4 * G4;

	t = FN_DECIMAL(0.6) - x0*x0 - y0*y0 - z0*z0 - w0*w0;
	if (t < 0) n0 = 0;
	else
	{
		t *= t;
		n0 = t * t * GradCoord4D(offset, i, j, k, l, x0, y0, z0, w0);
	}
	t = FN_DECIMAL(0.6) - x1*x1 - y1*y1 - z1*z1 - w1*w1;
	if (t < 0) n1 = 0;
	else
	{
		t *= t;
		n1 = t * t * GradCoord4D(offset, i + i1, j + j1, k + k1, l + l1, x1, y1, z1, w1);
	}
	t = FN_DECIMAL(0.6) - x2*x2 - y2*y2 - z2*z2 - w2*w2;
	if (t < 0) n2 = 0;
	else
	{
		t *= t;
		n2 = t * t * GradCoord4D(offset, i + i2, j + j2, k + k2, l + l2, x2, y2, z2, w2);
	}
	t = FN_DECIMAL(0.6) - x3*x3 - y3*y3 - z3*z3 - w3*w3;
	if (t < 0) n3 = 0;
	else
	{
		t *= t;
		n3 = t * t * GradCoord4D(offset, i + i3, j + j3, k + k3, l + l3, x3, y3, z3, w3);
	}
	t = FN_DECIMAL(0.6) - x4*x4 - y4*y4 - z4*z4 - w4*w4;
	if (t < 0) n4 = 0;
	else
	{
		t *= t;
		n4 = t * t * GradCoord4D(offset, i + 1, j + 1, k + 1, l + 1, x4, y4, z4, w4);
	}

	return 27 * (n0 + n1 + n2 + n3 + n4);
}

// Cubic Noise
FN_DECIMAL FastNoise::GetCubicFractal_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency, int octaves) const
{
	x *= frequency;
	y *= frequency;
	z *= frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleCubicFractalFBM_3D(x, y, z, octaves);
	case Billow:
		return SingleCubicFractalBillow_3D(x, y, z, octaves);
	case RigidMulti:
		return SingleCubicFractalRigidMulti_3D(x, y, z, octaves);
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::SingleCubicFractalFBM_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves) const
{
	FN_DECIMAL sum = SingleCubic_3D(m_perm[0], x, y, z);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += SingleCubic_3D(m_perm[i], x, y, z) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleCubicFractalBillow_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves) const
{
	FN_DECIMAL sum = FastAbs(SingleCubic_3D(m_perm[0], x, y, z)) * 2 - 1;
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SingleCubic_3D(m_perm[i], x, y, z)) * 2 - 1) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleCubicFractalRigidMulti_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, int octaves) const
{
	FN_DECIMAL sum = 1 - FastAbs(SingleCubic_3D(m_perm[0], x, y, z));
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum -= (1 - FastAbs(SingleCubic_3D(m_perm[i], x, y, z))) * amp;
	}

	return sum;
}

FN_DECIMAL FastNoise::GetCubic_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency) const
{
	return SingleCubic_3D(0, x * frequency, y * frequency, z * frequency);
}

const FN_DECIMAL CUBIC_3D_BOUNDING = 1 / (FN_DECIMAL(1.5) * FN_DECIMAL(1.5) * FN_DECIMAL(1.5));

FN_DECIMAL FastNoise::SingleCubic_3D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) const
{
	int x1 = FastFloor(x);
	int y1 = FastFloor(y);
	int z1 = FastFloor(z);

	int x0 = x1 - 1;
	int y0 = y1 - 1;
	int z0 = z1 - 1;
	int x2 = x1 + 1;
	int y2 = y1 + 1;
	int z2 = z1 + 1;
	int x3 = x1 + 2;
	int y3 = y1 + 2;
	int z3 = z1 + 2;

	FN_DECIMAL xs = x - (FN_DECIMAL)x1;
	FN_DECIMAL ys = y - (FN_DECIMAL)y1;
	FN_DECIMAL zs = z - (FN_DECIMAL)z1;

	return CubicLerp(
		CubicLerp(
			CubicLerp(ValCoord3DFast(offset, x0, y0, z0), ValCoord3DFast(offset, x1, y0, z0), ValCoord3DFast(offset, x2, y0, z0), ValCoord3DFast(offset, x3, y0, z0), xs),
			CubicLerp(ValCoord3DFast(offset, x0, y1, z0), ValCoord3DFast(offset, x1, y1, z0), ValCoord3DFast(offset, x2, y1, z0), ValCoord3DFast(offset, x3, y1, z0), xs),
			CubicLerp(ValCoord3DFast(offset, x0, y2, z0), ValCoord3DFast(offset, x1, y2, z0), ValCoord3DFast(offset, x2, y2, z0), ValCoord3DFast(offset, x3, y2, z0), xs),
			CubicLerp(ValCoord3DFast(offset, x0, y3, z0), ValCoord3DFast(offset, x1, y3, z0), ValCoord3DFast(offset, x2, y3, z0), ValCoord3DFast(offset, x3, y3, z0), xs),
			ys),
		CubicLerp(
			CubicLerp(ValCoord3DFast(offset, x0, y0, z1), ValCoord3DFast(offset, x1, y0, z1), ValCoord3DFast(offset, x2, y0, z1), ValCoord3DFast(offset, x3, y0, z1), xs),
			CubicLerp(ValCoord3DFast(offset, x0, y1, z1), ValCoord3DFast(offset, x1, y1, z1), ValCoord3DFast(offset, x2, y1, z1), ValCoord3DFast(offset, x3, y1, z1), xs),
			CubicLerp(ValCoord3DFast(offset, x0, y2, z1), ValCoord3DFast(offset, x1, y2, z1), ValCoord3DFast(offset, x2, y2, z1), ValCoord3DFast(offset, x3, y2, z1), xs),
			CubicLerp(ValCoord3DFast(offset, x0, y3, z1), ValCoord3DFast(offset, x1, y3, z1), ValCoord3DFast(offset, x2, y3, z1), ValCoord3DFast(offset, x3, y3, z1), xs),
			ys),
		CubicLerp(
			CubicLerp(ValCoord3DFast(offset, x0, y0, z2), ValCoord3DFast(offset, x1, y0, z2), ValCoord3DFast(offset, x2, y0, z2), ValCoord3DFast(offset, x3, y0, z2), xs),
			CubicLerp(ValCoord3DFast(offset, x0, y1, z2), ValCoord3DFast(offset, x1, y1, z2), ValCoord3DFast(offset, x2, y1, z2), ValCoord3DFast(offset, x3, y1, z2), xs),
			CubicLerp(ValCoord3DFast(offset, x0, y2, z2), ValCoord3DFast(offset, x1, y2, z2), ValCoord3DFast(offset, x2, y2, z2), ValCoord3DFast(offset, x3, y2, z2), xs),
			CubicLerp(ValCoord3DFast(offset, x0, y3, z2), ValCoord3DFast(offset, x1, y3, z2), ValCoord3DFast(offset, x2, y3, z2), ValCoord3DFast(offset, x3, y3, z2), xs),
			ys),
		CubicLerp(
			CubicLerp(ValCoord3DFast(offset, x0, y0, z3), ValCoord3DFast(offset, x1, y0, z3), ValCoord3DFast(offset, x2, y0, z3), ValCoord3DFast(offset, x3, y0, z3), xs),
			CubicLerp(ValCoord3DFast(offset, x0, y1, z3), ValCoord3DFast(offset, x1, y1, z3), ValCoord3DFast(offset, x2, y1, z3), ValCoord3DFast(offset, x3, y1, z3), xs),
			CubicLerp(ValCoord3DFast(offset, x0, y2, z3), ValCoord3DFast(offset, x1, y2, z3), ValCoord3DFast(offset, x2, y2, z3), ValCoord3DFast(offset, x3, y2, z3), xs),
			CubicLerp(ValCoord3DFast(offset, x0, y3, z3), ValCoord3DFast(offset, x1, y3, z3), ValCoord3DFast(offset, x2, y3, z3), ValCoord3DFast(offset, x3, y3, z3), xs),
			ys),
		zs) * CUBIC_3D_BOUNDING;
}


FN_DECIMAL FastNoise::GetCubicFractal_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency, int octaves) const
{
	x *= frequency;
	y *= frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleCubicFractalFBM_2D(x, y, octaves);
	case Billow:
		return SingleCubicFractalBillow_2D(x, y, octaves);
	case RigidMulti:
		return SingleCubicFractalRigidMulti_2D(x, y, octaves);
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::SingleCubicFractalFBM_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = SingleCubic_2D(m_perm[0], x, y);
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += SingleCubic_2D(m_perm[i], x, y) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleCubicFractalBillow_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = FastAbs(SingleCubic_2D(m_perm[0], x, y)) * 2 - 1;
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SingleCubic_2D(m_perm[i], x, y)) * 2 - 1) * amp;
	}

	return sum * m_fractalBounding;
}

FN_DECIMAL FastNoise::SingleCubicFractalRigidMulti_2D(FN_DECIMAL x, FN_DECIMAL y, int octaves) const
{
	FN_DECIMAL sum = 1 - FastAbs(SingleCubic_2D(m_perm[0], x, y));
	FN_DECIMAL amp = 1;
	int i = 0;

	while (++i < octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum -= (1 - FastAbs(SingleCubic_2D(m_perm[i], x, y))) * amp;
	}

	return sum;
}

FN_DECIMAL FastNoise::GetCubic_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency) const
{
	x *= frequency;
	y *= frequency;

	return SingleCubic_2D(0, x, y);
}

const FN_DECIMAL CUBIC_2D_BOUNDING = 1 / (FN_DECIMAL(1.5) * FN_DECIMAL(1.5));

FN_DECIMAL FastNoise::SingleCubic_2D(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y) const
{
	int x1 = FastFloor(x);
	int y1 = FastFloor(y);

	int x0 = x1 - 1;
	int y0 = y1 - 1;
	int x2 = x1 + 1;
	int y2 = y1 + 1;
	int x3 = x1 + 2;
	int y3 = y1 + 2;

	FN_DECIMAL xs = x - (FN_DECIMAL)x1;
	FN_DECIMAL ys = y - (FN_DECIMAL)y1;

	return CubicLerp(
		CubicLerp(ValCoord2DFast(offset, x0, y0), ValCoord2DFast(offset, x1, y0), ValCoord2DFast(offset, x2, y0), ValCoord2DFast(offset, x3, y0), xs),
		CubicLerp(ValCoord2DFast(offset, x0, y1), ValCoord2DFast(offset, x1, y1), ValCoord2DFast(offset, x2, y1), ValCoord2DFast(offset, x3, y1), xs),
		CubicLerp(ValCoord2DFast(offset, x0, y2), ValCoord2DFast(offset, x1, y2), ValCoord2DFast(offset, x2, y2), ValCoord2DFast(offset, x3, y2), xs),
		CubicLerp(ValCoord2DFast(offset, x0, y3), ValCoord2DFast(offset, x1, y3), ValCoord2DFast(offset, x2, y3), ValCoord2DFast(offset, x3, y3), xs),
		ys) * CUBIC_2D_BOUNDING;
}

// Cellular Noise
FN_DECIMAL FastNoise::GetCellular_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, float frequency) const
{
	x *= frequency;
	y *= frequency;
	z *= frequency;

	switch (m_cellularReturnType)
	{
	case CellValue:
	//case NoiseLookup:
	case Distance:
		return SingleCellular_3D(x, y, z);
	default:
		return SingleCellular2Edge_3D(x, y, z);
	}
}

FN_DECIMAL FastNoise::SingleCellular_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) const
{
	int xr = FastRound(x);
	int yr = FastRound(y);
	int zr = FastRound(z);

	FN_DECIMAL distance = 999999;
	int xc, yc, zc;

	switch (m_cellularDistanceFunction)
	{
	case Euclidean:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				for (int zi = zr - 1; zi <= zr + 1; zi++)
				{
					unsigned char lutPos = Index3D_256(0, xi, yi, zi);

					FN_DECIMAL vecX = xi - x + CELL_3D_X[lutPos] * m_cellularJitter;
					FN_DECIMAL vecY = yi - y + CELL_3D_Y[lutPos] * m_cellularJitter;
					FN_DECIMAL vecZ = zi - z + CELL_3D_Z[lutPos] * m_cellularJitter;

					FN_DECIMAL newDistance = vecX * vecX + vecY * vecY + vecZ * vecZ;

					if (newDistance < distance)
					{
						distance = newDistance;
						xc = xi;
						yc = yi;
						zc = zi;
					}
				}
			}
		}
		break;
	case Manhattan:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				for (int zi = zr - 1; zi <= zr + 1; zi++)
				{
					unsigned char lutPos = Index3D_256(0, xi, yi, zi);

					FN_DECIMAL vecX = xi - x + CELL_3D_X[lutPos] * m_cellularJitter;
					FN_DECIMAL vecY = yi - y + CELL_3D_Y[lutPos] * m_cellularJitter;
					FN_DECIMAL vecZ = zi - z + CELL_3D_Z[lutPos] * m_cellularJitter;

					FN_DECIMAL newDistance = FastAbs(vecX) + FastAbs(vecY) + FastAbs(vecZ);

					if (newDistance < distance)
					{
						distance = newDistance;
						xc = xi;
						yc = yi;
						zc = zi;
					}
				}
			}
		}
		break;
	case Natural:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				for (int zi = zr - 1; zi <= zr + 1; zi++)
				{
					unsigned char lutPos = Index3D_256(0, xi, yi, zi);

					FN_DECIMAL vecX = xi - x + CELL_3D_X[lutPos] * m_cellularJitter;
					FN_DECIMAL vecY = yi - y + CELL_3D_Y[lutPos] * m_cellularJitter;
					FN_DECIMAL vecZ = zi - z + CELL_3D_Z[lutPos] * m_cellularJitter;

					FN_DECIMAL newDistance = (FastAbs(vecX) + FastAbs(vecY) + FastAbs(vecZ)) + (vecX * vecX + vecY * vecY + vecZ * vecZ);

					if (newDistance < distance)
					{
						distance = newDistance;
						xc = xi;
						yc = yi;
						zc = zi;
					}
				}
			}
		}
		break;
	default:
		break;
	}

	//unsigned char lutPos;
	switch (m_cellularReturnType)
	{
	case CellValue:
		return ValCoord3D(m_seed, xc, yc, zc);

	/*case NoiseLookup:
		assert(m_cellularNoiseLookup);

		lutPos = Index3D_256(0, xc, yc, zc);
		return m_cellularNoiseLookup->GetNoise(xc + CELL_3D_X[lutPos] * m_cellularJitter, yc + CELL_3D_Y[lutPos] * m_cellularJitter, zc + CELL_3D_Z[lutPos] * m_cellularJitter);
*/
	case Distance:
		return distance;
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::SingleCellular2Edge_3D(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) const
{
	int xr = FastRound(x);
	int yr = FastRound(y);
	int zr = FastRound(z);

	FN_DECIMAL distance[FN_CELLULAR_INDEX_MAX + 1] = { 999999,999999,999999,999999 };

	switch (m_cellularDistanceFunction)
	{
	case Euclidean:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				for (int zi = zr - 1; zi <= zr + 1; zi++)
				{
					unsigned char lutPos = Index3D_256(0, xi, yi, zi);

					FN_DECIMAL vecX = xi - x + CELL_3D_X[lutPos] * m_cellularJitter;
					FN_DECIMAL vecY = yi - y + CELL_3D_Y[lutPos] * m_cellularJitter;
					FN_DECIMAL vecZ = zi - z + CELL_3D_Z[lutPos] * m_cellularJitter;

					FN_DECIMAL newDistance = vecX * vecX + vecY * vecY + vecZ * vecZ;

					for (int i = m_cellularDistanceIndex1; i > 0; i--)
						distance[i] = fmax(fmin(distance[i], newDistance), distance[i - 1]);
					distance[0] = fmin(distance[0], newDistance);
				}
			}
		}
		break;
	case Manhattan:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				for (int zi = zr - 1; zi <= zr + 1; zi++)
				{
					unsigned char lutPos = Index3D_256(0, xi, yi, zi);

					FN_DECIMAL vecX = xi - x + CELL_3D_X[lutPos] * m_cellularJitter;
					FN_DECIMAL vecY = yi - y + CELL_3D_Y[lutPos] * m_cellularJitter;
					FN_DECIMAL vecZ = zi - z + CELL_3D_Z[lutPos] * m_cellularJitter;

					FN_DECIMAL newDistance = FastAbs(vecX) + FastAbs(vecY) + FastAbs(vecZ);

					for (int i = m_cellularDistanceIndex1; i > 0; i--)
						distance[i] = fmax(fmin(distance[i], newDistance), distance[i - 1]);
					distance[0] = fmin(distance[0], newDistance);
				}
			}
		}
		break;
	case Natural:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				for (int zi = zr - 1; zi <= zr + 1; zi++)
				{
					unsigned char lutPos = Index3D_256(0, xi, yi, zi);

					FN_DECIMAL vecX = xi - x + CELL_3D_X[lutPos] * m_cellularJitter;
					FN_DECIMAL vecY = yi - y + CELL_3D_Y[lutPos] * m_cellularJitter;
					FN_DECIMAL vecZ = zi - z + CELL_3D_Z[lutPos] * m_cellularJitter;

					FN_DECIMAL newDistance = (FastAbs(vecX) + FastAbs(vecY) + FastAbs(vecZ)) + (vecX * vecX + vecY * vecY + vecZ * vecZ);

					for (int i = m_cellularDistanceIndex1; i > 0; i--)
						distance[i] = fmax(fmin(distance[i], newDistance), distance[i - 1]);
					distance[0] = fmin(distance[0], newDistance);
				}
			}
		}
		break;
	default:
		break;
	}

	switch (m_cellularReturnType)
	{
	case Distance2:
		return distance[m_cellularDistanceIndex1];
	case Distance2Add:
		return distance[m_cellularDistanceIndex1] + distance[m_cellularDistanceIndex0];
	case Distance2Sub:
		return distance[m_cellularDistanceIndex1] - distance[m_cellularDistanceIndex0];
	case Distance2Mul:
		return distance[m_cellularDistanceIndex1] * distance[m_cellularDistanceIndex0];
	case Distance2Div:
		return distance[m_cellularDistanceIndex0] / distance[m_cellularDistanceIndex1];
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::GetCellular_2D(FN_DECIMAL x, FN_DECIMAL y, float frequency) const
{
	x *= frequency;
	y *= frequency;

	switch (m_cellularReturnType)
	{
	case CellValue:
	//case NoiseLookup:
	case Distance:
		return SingleCellular_2D(x, y);
	default:
		return SingleCellular2Edge_2D(x, y);
	}
}

void FastNoise::GetVoronoi_2D(FN_DECIMAL x, FN_DECIMAL y, float m_jitter, FN_DECIMAL& out_x, FN_DECIMAL& out_y) const
{
	int xr = FastRound(x);
	int yr = FastRound(y);

	FN_DECIMAL distance = MAX_flt;

	switch (m_cellularDistanceFunction)
	{
	default:
	case Euclidean:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				FN_DECIMAL newX = xi + CELL_2D_X[lutPos] * m_jitter;
				FN_DECIMAL newY = yi + CELL_2D_Y[lutPos] * m_jitter;
				FN_DECIMAL vecX = x - newX;
				FN_DECIMAL vecY = y - newY;

				FN_DECIMAL newDistance = vecX * vecX + vecY * vecY;

				if (newDistance < distance)
				{
					distance = newDistance;
					out_x = newX;
					out_y = newY;
				}
			}
		}
		break;
	case Manhattan:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);
				
				FN_DECIMAL newX = xi + CELL_2D_X[lutPos] * m_jitter;
				FN_DECIMAL newY = yi + CELL_2D_Y[lutPos] * m_jitter;
				FN_DECIMAL vecX = x - newX;
				FN_DECIMAL vecY = y - newY;

				FN_DECIMAL newDistance = (FastAbs(vecX) + FastAbs(vecY));

				if (newDistance < distance)
				{
					distance = newDistance;
					out_x = newX;
					out_y = newY;
				}
			}
		}
		break;
	case Natural:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);
				
				FN_DECIMAL newX = xi + CELL_2D_X[lutPos] * m_jitter;
				FN_DECIMAL newY = yi + CELL_2D_Y[lutPos] * m_jitter;
				FN_DECIMAL vecX = x - newX;
				FN_DECIMAL vecY = y - newY;

				FN_DECIMAL newDistance = (FastAbs(vecX) + FastAbs(vecY)) + (vecX * vecX + vecY * vecY);

				if (newDistance < distance)
				{
					distance = newDistance;
					out_x = newX;
					out_y = newY;
				}
			}
		}
		break;
	}
}

void FastNoise::GetVoronoiNeighbors_2D(
	FN_DECIMAL x, FN_DECIMAL y,
	float m_jitter,
	FN_DECIMAL& out_x0, FN_DECIMAL& out_y0,
	FN_DECIMAL& out_x1, FN_DECIMAL& out_y1, FN_DECIMAL& out_distance1,
	FN_DECIMAL& out_x2, FN_DECIMAL& out_y2, FN_DECIMAL& out_distance2,
	FN_DECIMAL& out_x3, FN_DECIMAL& out_y3, FN_DECIMAL& out_distance3) const
{
	const int32 xr = FastRound(x);
	const int32 yr = FastRound(y);
	
	const FVector2D Position(x, y);
	
	FVector2D BestCenter;
	{
		FN_DECIMAL BestDistance = MAX_flt;

		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				const uint8 lutPos = Index2D_256(0, xi, yi);

				const FN_DECIMAL NewX = xi + CELL_2D_X[lutPos] * m_jitter;
				const FN_DECIMAL NewY = yi + CELL_2D_Y[lutPos] * m_jitter;
				const FVector2D Center(NewX, NewY);

				const FN_DECIMAL NewDistance = (Position - Center).SizeSquared();

				if (BestDistance > NewDistance)
				{
					BestDistance = NewDistance;
					BestCenter = Center;
				}
			}
		}
	}
	
	constexpr int32 num_dists = 4;
	FN_DECIMAL distance[num_dists] = { MAX_flt, MAX_flt, MAX_flt, MAX_flt };
	FN_DECIMAL xs[num_dists];
	FN_DECIMAL ys[num_dists];

	for (int xi = xr - 1; xi <= xr + 1; xi++)
	{
		for (int yi = yr - 1; yi <= yr + 1; yi++)
		{
			const uint8 lutPos = Index2D_256(0, xi, yi);

			FN_DECIMAL NewX = xi + CELL_2D_X[lutPos] * m_jitter;
			FN_DECIMAL NewY = yi + CELL_2D_Y[lutPos] * m_jitter;
			const FVector2D Center(NewX, NewY);

			FN_DECIMAL NewDistance = FMath::Abs(FVector2D::DotProduct(Position - (Center + BestCenter) / 2, (BestCenter - Center).GetSafeNormal()));

			for (int32 i = 0; i < num_dists; ++i)
			{
				if (distance[i] > NewDistance)
				{
					std::swap(distance[i], NewDistance);
					std::swap(xs[i], NewX);
					std::swap(ys[i], NewY);
				}
			}
		}
	}

	out_x0 = xs[0];
	out_x1 = xs[1];
	out_x2 = xs[2];
	out_x3 = xs[3];

	out_y0 = ys[0];
	out_y1 = ys[1];
	out_y2 = ys[2];
	out_y3 = ys[3];

	ensure(FMath::IsNearlyZero(distance[0]));
	// distance0 is always 0 as it's the distance to the border but we're inside out_distance0 = distance[0];
	out_distance1 = distance[1];
	out_distance2 = distance[2];
	out_distance3 = distance[3];
}

FN_DECIMAL FastNoise::SingleCellular_2D(FN_DECIMAL x, FN_DECIMAL y) const
{
	int xr = FastRound(x);
	int yr = FastRound(y);

	FN_DECIMAL distance = 999999;
	int xc, yc;

	switch (m_cellularDistanceFunction)
	{
	default:
	case Euclidean:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				FN_DECIMAL vecX = xi - x + CELL_2D_X[lutPos] * m_cellularJitter;
				FN_DECIMAL vecY = yi - y + CELL_2D_Y[lutPos] * m_cellularJitter;

				FN_DECIMAL newDistance = vecX * vecX + vecY * vecY;

				if (newDistance < distance)
				{
					distance = newDistance;
					xc = xi;
					yc = yi;
				}
			}
		}
		break;
	case Manhattan:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				FN_DECIMAL vecX = xi - x + CELL_2D_X[lutPos] * m_cellularJitter;
				FN_DECIMAL vecY = yi - y + CELL_2D_Y[lutPos] * m_cellularJitter;

				FN_DECIMAL newDistance = (FastAbs(vecX) + FastAbs(vecY));

				if (newDistance < distance)
				{
					distance = newDistance;
					xc = xi;
					yc = yi;
				}
			}
		}
		break;
	case Natural:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				FN_DECIMAL vecX = xi - x + CELL_2D_X[lutPos] * m_cellularJitter;
				FN_DECIMAL vecY = yi - y + CELL_2D_Y[lutPos] * m_cellularJitter;

				FN_DECIMAL newDistance = (FastAbs(vecX) + FastAbs(vecY)) + (vecX * vecX + vecY * vecY);

				if (newDistance < distance)
				{
					distance = newDistance;
					xc = xi;
					yc = yi;
				}
			}
		}
		break;
	}

	//unsigned char lutPos;
	switch (m_cellularReturnType)
	{
	case CellValue:
		return ValCoord2D(m_seed, xc, yc);

	/*case NoiseLookup:
		assert(m_cellularNoiseLookup);

		lutPos = Index2D_256(0, xc, yc);
		return m_cellularNoiseLookup->GetNoise(xc + CELL_2D_X[lutPos] * m_cellularJitter, yc + CELL_2D_Y[lutPos] * m_cellularJitter);*/

	case Distance:
		return distance;
	default:
		return 0;
	}
}

FN_DECIMAL FastNoise::SingleCellular2Edge_2D(FN_DECIMAL x, FN_DECIMAL y) const
{
	int xr = FastRound(x);
	int yr = FastRound(y);

	FN_DECIMAL distance[FN_CELLULAR_INDEX_MAX + 1] = { 999999,999999,999999,999999 };

	switch (m_cellularDistanceFunction)
	{
	default:
	case Euclidean:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				FN_DECIMAL vecX = xi - x + CELL_2D_X[lutPos] * m_cellularJitter;
				FN_DECIMAL vecY = yi - y + CELL_2D_Y[lutPos] * m_cellularJitter;

				FN_DECIMAL newDistance = vecX * vecX + vecY * vecY;

				for (int i = m_cellularDistanceIndex1; i > 0; i--)
					distance[i] = fmax(fmin(distance[i], newDistance), distance[i - 1]);
				distance[0] = fmin(distance[0], newDistance);
			}
		}
		break;
	case Manhattan:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				FN_DECIMAL vecX = xi - x + CELL_2D_X[lutPos] * m_cellularJitter;
				FN_DECIMAL vecY = yi - y + CELL_2D_Y[lutPos] * m_cellularJitter;

				FN_DECIMAL newDistance = FastAbs(vecX) + FastAbs(vecY);

				for (int i = m_cellularDistanceIndex1; i > 0; i--)
					distance[i] = fmax(fmin(distance[i], newDistance), distance[i - 1]);
				distance[0] = fmin(distance[0], newDistance);
			}
		}
		break;
	case Natural:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				FN_DECIMAL vecX = xi - x + CELL_2D_X[lutPos] * m_cellularJitter;
				FN_DECIMAL vecY = yi - y + CELL_2D_Y[lutPos] * m_cellularJitter;

				FN_DECIMAL newDistance = (FastAbs(vecX) + FastAbs(vecY)) + (vecX * vecX + vecY * vecY);

				for (int i = m_cellularDistanceIndex1; i > 0; i--)
					distance[i] = fmax(fmin(distance[i], newDistance), distance[i - 1]);
				distance[0] = fmin(distance[0], newDistance);
			}
		}
		break;
	}

	switch (m_cellularReturnType)
	{
	case Distance2:
		return distance[m_cellularDistanceIndex1];
	case Distance2Add:
		return distance[m_cellularDistanceIndex1] + distance[m_cellularDistanceIndex0];
	case Distance2Sub:
		return distance[m_cellularDistanceIndex1] - distance[m_cellularDistanceIndex0];
	case Distance2Mul:
		return distance[m_cellularDistanceIndex1] * distance[m_cellularDistanceIndex0];
	case Distance2Div:
		return distance[m_cellularDistanceIndex0] / distance[m_cellularDistanceIndex1];
	default:
		return 0;
	}
}

void FastNoise::GradientPerturb_3D(FN_DECIMAL& x, FN_DECIMAL& y, FN_DECIMAL& z, float frequency, float m_gradientPerturbAmp) const
{
	SingleGradientPerturb_3D(0, m_gradientPerturbAmp, frequency, x, y, z);
}

void FastNoise::GradientPerturbFractal_3D(FN_DECIMAL& x, FN_DECIMAL& y, FN_DECIMAL& z, float frequency, int octaves, float m_gradientPerturbAmp) const
{
	FN_DECIMAL amp = m_gradientPerturbAmp * m_fractalBounding;
	FN_DECIMAL freq = frequency;
	int i = 0;

	SingleGradientPerturb_3D(m_perm[0], amp, frequency, x, y, z);

	while (++i < octaves)
	{
		freq *= m_lacunarity;
		amp *= m_gain;
		SingleGradientPerturb_3D(m_perm[i], amp, freq, x, y, z);
	}
}

void FastNoise::SingleGradientPerturb_3D(unsigned char offset, FN_DECIMAL warpAmp, FN_DECIMAL frequency, FN_DECIMAL& x, FN_DECIMAL& y, FN_DECIMAL& z) const
{
	FN_DECIMAL xf = x * frequency;
	FN_DECIMAL yf = y * frequency;
	FN_DECIMAL zf = z * frequency;

	int x0 = FastFloor(xf);
	int y0 = FastFloor(yf);
	int z0 = FastFloor(zf);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	int z1 = z0 + 1;

	FN_DECIMAL xs, ys, zs;
	switch (m_interp)
	{
	default:
	case Linear:
		xs = xf - (FN_DECIMAL)x0;
		ys = yf - (FN_DECIMAL)y0;
		zs = zf - (FN_DECIMAL)z0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(xf - (FN_DECIMAL)x0);
		ys = InterpHermiteFunc(yf - (FN_DECIMAL)y0);
		zs = InterpHermiteFunc(zf - (FN_DECIMAL)z0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(xf - (FN_DECIMAL)x0);
		ys = InterpQuinticFunc(yf - (FN_DECIMAL)y0);
		zs = InterpQuinticFunc(zf - (FN_DECIMAL)z0);
		break;
	}

	int lutPos0 = Index3D_256(offset, x0, y0, z0);
	int lutPos1 = Index3D_256(offset, x1, y0, z0);

	FN_DECIMAL lx0x = Lerp(CELL_3D_X[lutPos0], CELL_3D_X[lutPos1], xs);
	FN_DECIMAL ly0x = Lerp(CELL_3D_Y[lutPos0], CELL_3D_Y[lutPos1], xs);
	FN_DECIMAL lz0x = Lerp(CELL_3D_Z[lutPos0], CELL_3D_Z[lutPos1], xs);

	lutPos0 = Index3D_256(offset, x0, y1, z0);
	lutPos1 = Index3D_256(offset, x1, y1, z0);

	FN_DECIMAL lx1x = Lerp(CELL_3D_X[lutPos0], CELL_3D_X[lutPos1], xs);
	FN_DECIMAL ly1x = Lerp(CELL_3D_Y[lutPos0], CELL_3D_Y[lutPos1], xs);
	FN_DECIMAL lz1x = Lerp(CELL_3D_Z[lutPos0], CELL_3D_Z[lutPos1], xs);

	FN_DECIMAL lx0y = Lerp(lx0x, lx1x, ys);
	FN_DECIMAL ly0y = Lerp(ly0x, ly1x, ys);
	FN_DECIMAL lz0y = Lerp(lz0x, lz1x, ys);

	lutPos0 = Index3D_256(offset, x0, y0, z1);
	lutPos1 = Index3D_256(offset, x1, y0, z1);

	lx0x = Lerp(CELL_3D_X[lutPos0], CELL_3D_X[lutPos1], xs);
	ly0x = Lerp(CELL_3D_Y[lutPos0], CELL_3D_Y[lutPos1], xs);
	lz0x = Lerp(CELL_3D_Z[lutPos0], CELL_3D_Z[lutPos1], xs);

	lutPos0 = Index3D_256(offset, x0, y1, z1);
	lutPos1 = Index3D_256(offset, x1, y1, z1);

	lx1x = Lerp(CELL_3D_X[lutPos0], CELL_3D_X[lutPos1], xs);
	ly1x = Lerp(CELL_3D_Y[lutPos0], CELL_3D_Y[lutPos1], xs);
	lz1x = Lerp(CELL_3D_Z[lutPos0], CELL_3D_Z[lutPos1], xs);

	x += Lerp(lx0y, Lerp(lx0x, lx1x, ys), zs) * warpAmp;
	y += Lerp(ly0y, Lerp(ly0x, ly1x, ys), zs) * warpAmp;
	z += Lerp(lz0y, Lerp(lz0x, lz1x, ys), zs) * warpAmp;
}

void FastNoise::GradientPerturb_2D(FN_DECIMAL& x, FN_DECIMAL& y, float frequency, float m_gradientPerturbAmp) const
{
	SingleGradientPerturb_2D(0, m_gradientPerturbAmp, frequency, x, y);
}

void FastNoise::GradientPerturbFractal_2D(FN_DECIMAL& x, FN_DECIMAL& y, float frequency, int octaves, float m_gradientPerturbAmp) const
{
	FN_DECIMAL amp = m_gradientPerturbAmp * m_fractalBounding;
	FN_DECIMAL freq = frequency;
	int i = 0;

	SingleGradientPerturb_2D(m_perm[0], amp, frequency, x, y);

	while (++i < octaves)
	{
		freq *= m_lacunarity;
		amp *= m_gain;
		SingleGradientPerturb_2D(m_perm[i], amp, freq, x, y);
	}
}

void FastNoise::SingleGradientPerturb_2D(unsigned char offset, FN_DECIMAL warpAmp, FN_DECIMAL frequency, FN_DECIMAL& x, FN_DECIMAL& y) const
{
	FN_DECIMAL xf = x * frequency;
	FN_DECIMAL yf = y * frequency;

	int x0 = FastFloor(xf);
	int y0 = FastFloor(yf);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	FN_DECIMAL xs, ys;
	switch (m_interp)
	{
	default:
	case Linear:
		xs = xf - (FN_DECIMAL)x0;
		ys = yf - (FN_DECIMAL)y0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(xf - (FN_DECIMAL)x0);
		ys = InterpHermiteFunc(yf - (FN_DECIMAL)y0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(xf - (FN_DECIMAL)x0);
		ys = InterpQuinticFunc(yf - (FN_DECIMAL)y0);
		break;
	}

	int lutPos0 = Index2D_256(offset, x0, y0);
	int lutPos1 = Index2D_256(offset, x1, y0);

	FN_DECIMAL lx0x = Lerp(CELL_2D_X[lutPos0], CELL_2D_X[lutPos1], xs);
	FN_DECIMAL ly0x = Lerp(CELL_2D_Y[lutPos0], CELL_2D_Y[lutPos1], xs);

	lutPos0 = Index2D_256(offset, x0, y1);
	lutPos1 = Index2D_256(offset, x1, y1);

	FN_DECIMAL lx1x = Lerp(CELL_2D_X[lutPos0], CELL_2D_X[lutPos1], xs);
	FN_DECIMAL ly1x = Lerp(CELL_2D_Y[lutPos0], CELL_2D_Y[lutPos1], xs);

	x += Lerp(lx0x, lx1x, ys) * warpAmp;
	y += Lerp(ly0x, ly1x, ys) * warpAmp;
}