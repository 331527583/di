//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[dynamic_bindings
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di  = boost::di;

//<-
enum eid { e1 = 1, e2 = 2 };
struct interface { virtual ~interface() noexcept = default; };
struct implementation1 : interface { };
struct implementation2 : interface { };
//->

class dynamic_bindings {
public:
    dynamic_bindings& operator=(const dynamic_bindings&) = delete;

    explicit dynamic_bindings(eid& id)
        : id(id)
    { }

    /*<<module configuration>>*/
    auto configure() const {
        return di::make_injector(
            /*<<bind `interface` to lazy lambda expression>>*/
            di::bind<interface>().to(
                [&](const auto& injector) -> std::shared_ptr<interface> {
                    switch(id) {
                        default: return nullptr;
                        case e1: return (const std::shared_ptr<implementation1>&)injector;
                        case e2: return (const std::shared_ptr<implementation2>&)injector;
                    }

                    return nullptr;
                }
            )
        );
    }

private:
    eid& id;
};

int main() {
    auto id = e1;

    /*<<create interface with `id = e1`>>*/
    auto injector = di::make_injector(dynamic_bindings{id});
    assert(dynamic_cast<implementation1*>(injector.create<std::shared_ptr<interface>>().get()));

    id = e2;
    /*<<create interface with `id = e2`>>*/
    assert(dynamic_cast<implementation2*>(injector.create<std::shared_ptr<interface>>().get()));
    (void)id;
}

//]

