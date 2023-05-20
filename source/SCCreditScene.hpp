//
//  SCCreditScene.hpp
//  sunk-ios
//
//  Created by chloe on 5/19/23.
//

#ifndef SCCreditScene_hpp
#define SCCreditScene_hpp

#include <stdio.h>
#include <cugl/cugl.h>

class CreditScene : public cugl::Scene2 {
public:
    enum Choice {
      NONE,
      LOAD
    };
    
protected:
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::scene2::Button> _backButton;
    
    Choice _choice;
    
public:
    
    CreditScene() : cugl::Scene2() {}
    
    ~CreditScene() { dispose(); }
    
    void dispose() override;
    
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    virtual void setActive(bool value) override;
    
    Choice getChoice() const { return _choice; }
    
};

#endif /* SCCreditScene_hpp */
