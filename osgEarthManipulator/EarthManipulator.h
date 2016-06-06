/*
    y=a-b*c^x a=18 b=63809 c =0.78
*/
#ifndef OSGEARTHUTIL_EARTHMANIPULATOR
#define OSGEARTHUTIL_EARTHMANIPULATOR

//#include <osgEarthUtil/Common>
#include "Export"
#include "Notify"
#include "optional"

//#include <osgEarth/Common>
#include "Radial.h"
#include "Viewpoint"
#include "Revisioning"
//#include "INavigatorManager.h"
#include <osg/Timer>
#include <osg/ObserverNodePath>
#include <osgGA/CameraManipulator>
#include <map>
#include <list>
#include <utility>
#include <iostream>
using namespace std;

#define USE_OBSERVER_NODE_PATH 1

namespace osgEarth { namespace Util
{
    using namespace osgEarth;

    class OSGEARTH_EXPORT EarthManipulator : public osgGA::CameraManipulator
	{
    public:

        enum ActionType {
            ACTION_NULL,
            ACTION_HOME,
            ACTION_GOTO,
            ACTION_PAN,
            ACTION_PAN_LEFT,
            ACTION_PAN_RIGHT,
            ACTION_PAN_UP,
            ACTION_PAN_DOWN,
            ACTION_ROTATE,
            ACTION_ROTATE_LEFT,
            ACTION_ROTATE_RIGHT,
            ACTION_ROTATE_UP,
            ACTION_ROTATE_DOWN,
            ACTION_ZOOM,
            ACTION_ZOOM_IN,
            ACTION_ZOOM_OUT,
            ACTION_EARTH_DRAG
        };

        typedef std::vector<ActionType> ActionTypeVector;

        enum EventType {
            EVENT_MOUSE_CLICK        = osgGA::GUIEventAdapter::USER << 1,
            EVENT_MOUSE_DOUBLE_CLICK = osgGA::GUIEventAdapter::DOUBLECLICK,
            EVENT_MOUSE_DRAG         = osgGA::GUIEventAdapter::DRAG,
            EVENT_KEY_DOWN           = osgGA::GUIEventAdapter::KEYDOWN,
            EVENT_SCROLL             = osgGA::GUIEventAdapter::SCROLL
        };

        enum MouseEvent {
            MOUSE_LEFT_BUTTON   = osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON,
            MOUSE_MIDDLE_BUTTON = osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON,
            MOUSE_RIGHT_BUTTON  = osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON
        };

        enum ActionOptionType {            
            OPTION_SCALE_X,            
            OPTION_SCALE_Y,            
            OPTION_CONTINUOUS,          
            OPTION_SINGLE_AXIS,         
            OPTION_GOTO_RANGE_FACTOR,   
            OPTION_DURATION             
        };

        enum TetherMode
        {
            TETHER_CENTER,              
            TETHER_CENTER_AND_ROTATION, 
            TETHER_CENTER_AND_HEADING  
        };

        enum CameraProjection
        {
            PROJ_PERSPECTIVE,
            PROJ_ORTHOGRAPHIC
        };

        struct OSGEARTH_EXPORT ActionOption {
            ActionOption() { }
            ActionOption( int option, bool value ) : _option(option), _bool_value(value) { }
            ActionOption( int option, int value ) : _option(option), _int_value(value) { }
            ActionOption( int option, double value ) : _option(option), _dbl_value(value) { }

            int option() const { return _option; }
            bool boolValue() const { return _bool_value; }
            int intValue() const { return _int_value; }
            double doubleValue() const { return _dbl_value; }

        private:
            int _option;
            union {
                bool _bool_value;
                int _int_value;
                double _dbl_value;
            };
        };

        struct OSGEARTH_EXPORT ActionOptions : public std::vector<ActionOption> {
            void add( int option, bool value ) { push_back( ActionOption(option,value) ); }
            void add( int option, int value )  { push_back( ActionOption(option,value) ); }
            void add( int option, double value) { push_back( ActionOption(option,value) ); }
        };

    protected:
        struct InputSpec 
        {
            InputSpec( int event_type, int input_mask, int modkey_mask )
                : _event_type(event_type), _input_mask(input_mask), _modkey_mask( modkey_mask ) { }
            InputSpec( const InputSpec& rhs )
                : _event_type(rhs._event_type), _input_mask(rhs._input_mask), _modkey_mask(rhs._modkey_mask) { }

            bool operator == ( const InputSpec& rhs ) const {
                return _event_type == rhs._event_type && 
                       _input_mask == rhs._input_mask && 
                       ((_modkey_mask|osgGA::GUIEventAdapter::MODKEY_NUM_LOCK) == (rhs._modkey_mask|osgGA::GUIEventAdapter::MODKEY_NUM_LOCK));
            }

            inline bool operator < (const InputSpec& rhs) const {
                if ( _event_type < rhs._event_type) return true;
                else if ( _event_type > rhs._event_type ) return false;
                else if ( _input_mask < rhs._input_mask ) return true;
                else if ( _input_mask > rhs._input_mask ) return false;
                else return ( _modkey_mask < rhs._modkey_mask );
            }

            int _event_type;
            int _input_mask;
            int _modkey_mask;
        };
        typedef std::list<InputSpec> InputSpecs;

        enum Direction {
            DIR_NA,
            DIR_LEFT,
            DIR_RIGHT,
            DIR_UP,
            DIR_DOWN
        };

        struct Action
        {
            Action( ActionType type =ACTION_NULL );
            Action( ActionType type, const ActionOptions& options );
            Action( const Action& rhs );
            ActionType _type;
            Direction _dir;
            ActionOptions _options;
            bool getBoolOption( int option, bool defaultValue ) const;
            int getIntOption( int option, int defaultValue ) const;
            double getDoubleOption( int option, double defaultValue ) const;
        private:
            void init();
        };

        void dumpActionInfo( const Action& action, osg::NotifySeverity level ) const;
        
        static Action NullAction;

    public:

        class OSGEARTH_EXPORT Settings : public osg::Referenced, public Revisioned
        {
        public:
            Settings();

            Settings( const Settings& rhs );
       
            virtual ~Settings() { }
       
            void bindMouse(
                ActionType action, int button_mask,
                int modkey_mask = 0L,
                const ActionOptions& options =ActionOptions() );

            void bindMouseClick(
                ActionType action, int button_mask,
                int modkey_mask =0L,
                const ActionOptions& options =ActionOptions() );

            void bindMouseDoubleClick(
                ActionType action, int button_mask,
                int modkey_mask =0L,
                const ActionOptions& options =ActionOptions() );

            void bindKey(
                ActionType action, int key,
                int modkey_mask =0L,
                const ActionOptions& options =ActionOptions() );

            void bindScroll(
                ActionType action, int scrolling_motion,
                int modkey_mask =0L,
                const ActionOptions& options =ActionOptions() );

            void setMouseSensitivity( double value ) { _mouse_sens = value; }

            double getMouseSensitivity() const { return _mouse_sens; }

            void setKeyboardSensitivity( double value ) { _keyboard_sens = value; }

            double getKeyboardSensitivity() const { return _keyboard_sens; }

            void setScrollSensitivity( double value ) { _scroll_sens = value; }

            double getScrollSensitivity() const { return _scroll_sens; }

            void setSingleAxisRotation( bool value ) { _single_axis_rotation = value; }

            bool getSingleAxisRotation() const { return _single_axis_rotation; }

            void setLockAzimuthWhilePanning( bool value ) { _lock_azim_while_panning = value; }

            bool getLockAzimuthWhilePanning() const { return _lock_azim_while_panning; }

            void setMinMaxPitch( double min_pitch, double max_pitch );

            double getMinPitch() const { return _min_pitch; }

            double getMaxPitch() const { return _max_pitch; }        

            double getMaxXOffset() const { return _max_x_offset; }

            double getMaxYOffset() const { return _max_y_offset; }

            double getMinDistance() const {return _min_distance; }
			double getMaxDistance() const {return _max_distance; }

            void setMinMaxDistance( double min_distance, double max_distance);

            void setMaxOffset(double max_x_offset, double max_y_offset);

            TetherMode getTetherMode() const { return _tether_mode; }

            void setTetherMode( TetherMode tether_mode ) { _tether_mode = tether_mode; }

            ActionTypeVector& getBreakTetherActions() { return _breakTetherActions; }
            const ActionTypeVector& getBreakTetherActions() const { return _breakTetherActions; }

            void setArcViewpointTransitions( bool value );
            bool getArcViewpointTransitions() const { return _arc_viewpoints; }

            void setAutoViewpointDurationEnabled( bool value );
            bool getAutoViewpointDurationEnabled() const { return _auto_vp_duration; }

            void setAutoViewpointDurationLimits( double minSeconds, double maxSeconds );
            void getAutoViewpointDurationLimits( double& out_minSeconds, double& out_maxSeconds ) const {
                out_minSeconds = _min_vp_duration_s;
                out_maxSeconds = _max_vp_duration_s;
            }

            void setCameraProjection( const CameraProjection& value );
            const CameraProjection& getCameraProjection() const { return _camProjType; }

            void setCameraFrustumOffsets( const osg::Vec2s& offsets );
            const osg::Vec2s& getCameraFrustumOffsets() const { return _camFrustOffsets; }            

        private:

            friend class EarthManipulator;

            typedef std::pair<InputSpec,Action> ActionBinding;
            typedef std::map<InputSpec,Action> ActionBindings;

            const Action& getAction( int event_type, int input_mask, int modkey_mask ) const;

            void expandSpec( const InputSpec& input, InputSpecs& output ) const;
            void bind( const InputSpec& spec, const Action& action );

        private:

            ActionBindings _bindings;
            bool _single_axis_rotation;
            bool _lock_azim_while_panning;
            double _mouse_sens;
            double _keyboard_sens;
            double _scroll_sens;
            double _min_pitch;
            double _max_pitch;

            double _max_x_offset;
            double _max_y_offset;

            double _min_distance;
            double _max_distance;

            TetherMode _tether_mode;
            ActionTypeVector _breakTetherActions;
            bool _arc_viewpoints;
            bool _auto_vp_duration;
            double _min_vp_duration_s, _max_vp_duration_s;

            CameraProjection _camProjType;
            osg::Vec2s _camFrustOffsets;			
        };

    public:
        EarthManipulator();
        EarthManipulator( const EarthManipulator& rhs );

        void applySettings( Settings* settings );

        Settings* getSettings() const;

        Viewpoint getViewpoint() const;

        virtual void setViewpoint( const Viewpoint& vp, double duration_s =0.0 );

        void cancelViewpointTransition() { _setting_viewpoint = false; }

        void setHomeViewpoint( const Viewpoint& vp, double duration_s = 0.0 );

        void setTetherNode( osg::Node* node );

        osg::Node* getTetherNode() const;

        virtual void pan( double dx, double dy );
		void handleKeyboardPan(double dx, double dy);
		void handlePan(double dx, double dy ,osg::View* view);
		void computerThrowSpeed(osg::View* view);
		float tb_project_to_sphere( float r, float x, float y );
		void getPoint(double dx, double dy,osg::Vec3d& out_coords,osg::View* view);
		void handleThrown();
		float getEarthRadius();
		Radial3 getCameraScreenRadial(const osg::Camera *pCamera, const osg::Vec2 &ptPosNormalize);
		bool	getHitTestPoint(const Radial3 &ray, const osg::Plane &plane, osg::Vec3 &vecRet, bool bIngoreNegtive = true);
		void printVec(osg::Vec3d vec, string name){ cout<<name<<" "<<vec._v[0]<<" "<<vec._v[1]<<" "<<vec._v[2]<<endl; }
		void panWithZoom(float length);
		double getDistanceFromZoomFormula();
		double getZoomFromulaXFactorFromDistance();
        virtual void rotate( double dx, double dy );

        virtual void zoom( double dx, double dy);

        virtual void drag( double dx, double dy, osg::View* view);

        bool screenToWorld(float x, float y, osg::View* view, osg::Vec3d& out_coords ) const;

        double getDistance() const { return _distance; }

        void   setDistance( double distance);

     
        const osg::Quat& getRotation() { return _rotation; }

        void  setRotation( const osg::Quat& rotation) { _rotation = rotation; }



    public: // osgGA::MatrixManipulator

        virtual const char* className() const { return "EarthManipulator"; }

        virtual void setByMatrix(const osg::Matrixd& matrix);

        virtual void setByInverseMatrix(const osg::Matrixd& matrix) { setByMatrix(osg::Matrixd::inverse(matrix)); }

        virtual osg::Matrixd getMatrix() const;

        virtual osg::Matrixd getInverseMatrix() const;

        virtual osgUtil::SceneView::FusionDistanceMode getFusionDistanceMode() const { return osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE; }

        virtual float getFusionDistanceValue() const { return _distance; }

        virtual void setNode(osg::Node*);

        virtual osg::Node* getNode();

        virtual void home(double /*unused*/);
        virtual void home(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

        virtual void init(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

        virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

        virtual void getUsage(osg::ApplicationUsage& usage) const;

        virtual void computeHomePosition();


    protected:

        virtual ~EarthManipulator();
        
        bool intersect(const osg::Vec3d& start, const osg::Vec3d& end, osg::Vec3d& intersection) const;

        void resetMouse( osgGA::GUIActionAdapter& );

        void flushMouseEventStack();

        void addMouseEvent(const osgGA::GUIEventAdapter& ea);

        void setByLookAt(const osg::Vec3d& eye, const osg::Vec3d& lv, const osg::Vec3d& up);

        bool handleAction( const Action& action, double dx, double dy, double duration );

        virtual bool handleMouseAction( const Action& action, osg::View* view );
        virtual bool handleMouseClickAction( const Action& action );
        virtual bool handleKeyboardAction( const Action& action, double duration_s = DBL_MAX );
        virtual bool handleScrollAction( const Action& action, double duration_s = DBL_MAX );
        virtual bool handlePointAction( const Action& type, float mx, float my, osg::View* view );
        virtual void handleContinuousAction( const Action& action, osg::View* view );
        virtual void handleMovementAction( const ActionType& type, double dx, double dy, osg::View* view );

        bool isMouseMoving();

        void recalculateRoll();

    protected:

        enum TaskType
        {
            TASK_NONE,
            TASK_PAN,
            TASK_ROTATE,
            TASK_ZOOM
        };

        struct Task : public osg::Referenced
        {
            Task() : _type(TASK_NONE) { }
            void set( TaskType type, double dx, double dy, double duration, double now ) {
                _type = type; _dx = dx; _dy = dy; _duration_s = duration; _time_last_service = now;
            }
            TaskType _type;
            double   _dx, _dy;
            double   _duration_s;
            double   _time_last_service;
        };

        bool serviceTask();

        void getLocalEulerAngles( double* out_azim, double* out_pitch =0L ) const;


        void recalculateCenter( const osg::CoordinateFrame& frame );

        osg::Matrixd getRotation(const osg::Vec3d& center) const;
        osg::Quat makeCenterRotation(const osg::Vec3d& center) const
        {
            return getRotation(center).getRotate().inverse();
        }

        void updateTether();

        void updateSetViewpoint();

        void updateHandCam( const osg::Timer_t& now );

        bool isMouseClick( const osgGA::GUIEventAdapter* mouse_up_event ) const;
        
        void applyOptionsToDeltas( const Action& action, double& dx, double& dy );

        void configureDefaultSettings();

        void reinitialize();

        bool established();

        void setCenter( const osg::Vec3d& center );

        bool createLocalCoordFrame( const osg::Vec3d& worldPos, osg::CoordinateFrame& out_frame ) const;

        ActionType getActionTypeForEvent( const osgGA::GUIEventAdapter& ea ) const;

    public:
            
        void recalculateCenter() { recalculateCenter(_centerLocalToWorld); }

        //const GeoPoint& centerMap() const { return _centerMap; }
#if 0
			//接口实现
	public:
		virtual bool        initialize(void){return true;} ;

		virtual unsigned    getNavigators(std::vector<std::string> &vecNavigatorNames) const{return 0u;};

		virtual bool        setActiveNavigator(const std::string &strName) {return false;};
		virtual std::string getActiveNavigator(void) const {return "";};

		virtual NavigationParam        *getNavigationParam(void) {return NULL;};
		virtual const NavigationParam  *getNavigationParam(void) const {return NULL;};

		virtual CoordinateParam        *getCoordinateParam(void) {return NULL;};
		virtual const CoordinateParam  *getCoordinateParam(void) const {return NULL;};

		virtual void        resetCamera(void) {};
		virtual void        stopInertia(void) {};

		virtual void        getCameraPose(CameraPose &pose) const;
		virtual void        setCameraPose(const CameraPose &pose);
#endif
    protected:
        // makeshift "stack" of the last 2 incoming events.
        osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t1;
        osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t0;

        osg::ref_ptr<const osgGA::GUIEventAdapter> _mouse_down_event;

        osg::observer_ptr<osg::Node> _node;
        osg::observer_ptr<osg::CoordinateSystemNode> _csn;
#ifdef USE_OBSERVER_NODE_PATH
        osg::ObserverNodePath _csnObserverPath;
#endif
        osg::NodePath _csnPath;

        //osg::ref_ptr<const osgEarth::SpatialReference> _cached_srs;
        bool _is_geocentric;
        bool _srs_lookup_failed;

        osg::observer_ptr<osg::Node> _tether_node;
        osg::Transform*              _tether_xform;
        osg::Vec3d                   _tether_local_center;
        Viewpoint                    _pre_tether_vp;

        double                  _time_s_last_frame;
        double                  _time_s_now;
        osg::Timer_t            _now;
        double                  _delta_t;
        double                  _t_factor;
        bool                    _thrown;
        osg::Vec3d              _center;
        //GeoPoint                _centerMap;

        osg::CoordinateFrame    _centerLocalToWorld;
        osg::Quat               _rotation;

        osg::Quat               _centerRotation;
        double                  _distance;
        double                  _offset_x;
        double                  _offset_y;
        osg::Vec3d              _previousUp;
        osg::ref_ptr<Task>      _task;
        osg::Timer_t            _time_last_frame;
        //double                  _local_pitch;
        //double                  _local_azim;

        bool                    _continuous;
        double                  _continuous_dx;
        double                  _continuous_dy;
        double                  _last_continuous_action_time;

        double                  _single_axis_x;
        double                  _single_axis_y;

        bool                    _has_pending_viewpoint;
        Viewpoint               _pending_viewpoint;
        double                  _pending_viewpoint_duration_s;

        bool                    _setting_viewpoint;
        Viewpoint               _start_viewpoint;
        double                  _delta_heading, _delta_pitch, _delta_range, _arc_height;
        osg::Vec3d              _delta_focal_point;
        double                  _time_s_set_viewpoint;
        double                  _set_viewpoint_duration_s;
        double                  _set_viewpoint_accel;
        double                  _set_viewpoint_accel_2;

        unsigned                _frame_count;

        osg::ref_ptr<Settings> _settings;		

        osgEarth::optional<Viewpoint> _homeViewpoint;
        double _homeViewpointDuration;

        Action _last_action;

        osg::observer_ptr<osg::Camera> _viewCamera;
        double _vfov;
        double _tanHalfVFOV;
        optional<osg::CullSettings::ComputeNearFarMode> _savedCNFMode;
        Revision _viewCameraSettingsMonitor;
        
        struct CameraPostUpdateCallback : public osg::NodeCallback {
            CameraPostUpdateCallback(EarthManipulator* m) : _m(m) { }
            virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) { _m->postUpdate(); traverse(node,nv); }
            EarthManipulator* _m;
        };
        void postUpdate();
        friend struct CameraUpdateCallback;

        osg::observer_ptr<CameraPostUpdateCallback> _cameraUpdateCB;

        void updateCamera( osg::Camera* eventCamera );
        
        osg::Vec3d               _lastPointOnEarth;

		osg::Vec3d               _lastNormal;
		double                   _lastAngle;
		bool                     _isPan;
		float                    _lastDragTime;
		float                    _lastReleaseTime;
		double					 _zoomFactor;
		double                   _zoomSensitivity;
		osg::View*               _zoomView;
        //osg::ref_ptr< TerrainCallback > _terrainCallback;
    };

} } // namespace osgEarth::Util

#endif // OSGEARTHUTIL_EARTHMANIPULATOR
